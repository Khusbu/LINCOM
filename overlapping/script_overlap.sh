#!/bin/bash
if (( $# == 0 )); then
  echo "Give filename of edgelist as a command line argument to the script"
  exit
fi
filename=$1

cover=$filename"_cover_i.txt"
adjlist="Adjlist_"$filename
membershipFile="Membership_"$filename

gcc LINCOM_NSV.c -o output
gcc Nicosia_f.c -lm -o modularity
gcc changeLouvain.c -o change
gcc cov_gen.c -o cover_generator

report="overlapping_ins"

if [ -e $report ]
then
  rm $report
fi

for i in $(seq 0.4 0.05 0.9)
do
	echo r = $i
	numberOfComm=`./output $filename $i | tee /dev/tty | awk -F': ' '{print $2}'`
	if (($? != 0)); then
    echo "seg fault in LINCOM, breaking out"
    echo
    break
  fi

	modularity_output=`./modularity $adjlist $cover`
	echo "Initial Overlapping modularity: `echo "$modularity_output" | awk -F': ' 'NR==1{print $2}'`"

	nodeCount=`echo "$modularity_output" | awk -F': ' 'NR==2{print $2}'`
	mv $cover $i$cover

	input="louvain_input_"$filename;
	if [ -s $input ]
	then
		./convert -i $input -o graph.bin
		./community graph.bin -l -1 -v > graph.tree 2>/dev/null
		hierarchy_output=`./hierarchy graph.tree`
		level=`echo "$hierarchy_output"| awk -F': ' 'NR==1{print $2}'`
		let level=$level-1

		./hierarchy graph.tree -l $level > "final_cover"$i$filename
		detectedComm=`./change "final_cover"$i$filename "FINAL"$i$filename $numberOfComm | awk -F': ' '{print $2}'`
		echo 'Number of communities after mod max: '$detectedComm

		./cover_generator $numberOfComm $nodeCount $membershipFile "FINAL"$i$filename "COVER"$i$filename $detectedComm
    overlapping_modularity=`./modularity $adjlist "COVER"$i$filename | awk -F': ' 'NR==1{print $2}'`
		echo "Final Modularity: "$overlapping_modularity

    echo $i $detectedComm $overlapping_modularity >> $report

		rm graph.tree
		rm graph.bin
		rm "final_cover"$i$filename
		rm "FINAL"$i$filename
		rm $membershipFile
		rm "COVER"$i$filename
	else
		echo Only one community
    echo $i $numberOfComm 0.000000 >> $report
	fi
  rm $i$cover
  rm $input
  rm "Mapping"$filename
  echo
  echo
done
rm $adjlist
