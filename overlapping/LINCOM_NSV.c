
/*The program assumes that the node label starts from 1 and the graph has no missing nodes.*/


#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#define MAX 50

typedef struct vertex
{
    int label;
    float score;
    int community;
    int parent;
    int category;//1-->Broker 2-->Community Node 3--> Broker node turned community node
}node;
typedef struct comm
{
    int value;
    int label;
}comm;

typedef struct record
{
    int label;
    int category;
    int mark1;
    int influenced;
    int traversed;
}record;

typedef struct brok
{
    int label;
    int parent;
}brok;

typedef struct linked_list
{
    int label;
    int mark;
    int test;
    struct linked_list *ptr;
}link;
void sort();
float val=0.0;
node *Queue,*Broker;
int *degC,*list,*list1,*sort1,no_of_overlap=0;
link **Overlap;
int abc=0, flag1=1;
int max_label,count_of_comm,*degree,**adlist,noOfComm,**arraySet,**contribute;
record *labels;
void maxCMT(int);
void maxLabel(FILE *);
void degreeOfNodes(FILE *);
void adjacencyList(FILE *);
void generateNodes();
void community_detection(float);
float calculate_score(int);
int print();
int neighbour(int,int);
void convergence();
int no_of_comm1=0,min_deg_node;
int **arrayL;
char filename[50]="";
int main( int argc, char **argv)
{
    	clock_t begin,end,total_begin,total_end;
    	total_begin=clock();
	char str[MAX];
	FILE *fp;
	float t;
	int temp,i,j;
	if(argc != 3)
	{
		printf("Wrong command line args");
		exit(0);
	}
	else
	{
		strcat(filename, argv[1]);
	}
	//printf("\nEnter the filename along with extension: ");
	//scanf("%s",filename);
	//char filename[50]="Epinions.txt";
	fp=fopen(filename,"r");
	if(fp==NULL)
	{
		printf("File does not exist !!");
		exit(0);
	}
	begin=clock();
	maxLabel(fp);
	//printf("\n\nThe maximum label of the graph is %d",max_label);
	end=clock();
	//printf("\n\nTime taken to calculate Max Label = %f\n",(float)(end-begin)/CLOCKS_PER_SEC);
	fclose(fp);
	fp=fopen(filename,"r");
	begin=clock();
	degreeOfNodes(fp);
	//printf("\n\nThe degree of nodes calculated!!");
	end=clock();
	//printf("\n\nTime taken to calculate degrees of nodes = %f\n",(float)(end-begin)/CLOCKS_PER_SEC);
	fclose(fp);
        fp=fopen(filename,"r");
	//rewind(fp);
	begin=clock();
	adjacencyList(fp);
	end=clock();
	//printf("\n\nThe adjacency list calculated!!");
	//printf("\n\nTime taken to calculate adjacency list = %f\n",(float)(end-begin)/CLOCKS_PER_SEC);
	fclose(fp);
	//printf("Min degree node is %d\n",min_deg_node);
	//t=0.75;
t = atof(argv[2]);
	Queue=(node *)calloc(max_label+2,sizeof(node));
	Broker=(node *)calloc(max_label+2,sizeof(node));
    	Overlap=(link**)calloc(max_label+2,sizeof(link*));
    	labels=(record *)calloc(max_label+2,sizeof(record));

	node start;
    	start.label=min_deg_node;
    	start.score=0;
    	start.parent=start.label;
    	start.community=min_deg_node;
    	start.category=1;//broker
    	labels[start.label].influenced=1;

    	Broker[0]=start;
    	labels[start.label].label=start.label;
    	labels[start.label].category=1;
    	community_detection(t);

    	begin=clock();
    	convergence();
    	end=clock();
	free(labels);
    	//printf("\n\nTime taken for convergence = %f\n",(float)(end-begin)/CLOCKS_PER_SEC);
	begin=clock();
  no_of_comm1=print();
	printf("No. of communities before mod max : %d\n",no_of_comm1);
	end=clock();
	//printf("\n\nTime taken for print =%f\n",(float)(end-begin)/CLOCKS_PER_SEC);
	begin=clock();
	generateNodes();
	end=clock();
	//printf("\n\nTime taken for reduction = %f\n",(float)(end-begin)/CLOCKS_PER_SEC);
    	total_end=clock();
        //printf("\n\nTotal time taken for LINCOM = %f\n",(float)(total_end-total_begin)/CLOCKS_PER_SEC);
	free(adlist[0]);
    	for(i=1;i<=max_label+1;i++)
    	{
    	    link *temp=Overlap[i];
    	    while(temp!=NULL)
    	    {
                link *p=temp;
                temp=temp->ptr;
                free(p);
    	    }

            free(adlist[i]);
    	}
	free(Overlap);
    	free(adlist);
	return 0;
}

/*Finds the maximum no. of nodes present in the graph*/

void maxLabel(FILE *f)
{
	max_label=0;
	int i,j;
    	while(fscanf(f,"%d %d",&i,&j)!=EOF)
    	{
        	if(i>max_label && i>j)
        	     max_label= i;
        	else if(j>max_label && j>i)
			max_label= j;
    	}
}

/*Calculates the degree of the nodes present in the graph*/

void degreeOfNodes(FILE *f)
{
    int n1,n2;
    degree=(int*)calloc((max_label+2),sizeof(int));
    while(fscanf(f,"%d %d",&n1,&n2)!=EOF)
    {
        degree[n1]++;
        degree[n2]++;
    }
}

/*Calculates the adjacency list of the graph and minimum degree node present*/
void adjacencyList(FILE *f)
{
	int n1,n2,*flag,i,j,cd=0,min=degree[1];min_deg_node=1;
    	adlist=(int**)calloc((max_label+2),sizeof(int*));

	char name[50] ="Adjlist_";
	strcat(name,filename);
	FILE *ad;
	ad=fopen(name,"w+");

    	for(i=1;i<=max_label;i++)
    	{
		if(degree[i]<min && i!=0)
		{
			min=degree[i];
			min_deg_node=i;
		}
        	adlist[i]=(int*)calloc((degree[i]+2),sizeof(int));
		adlist[i][0]=degree[i];
    	}
    	flag=(int*)calloc((max_label+2),sizeof(int));
    	while(fscanf(f,"%d %d",&n1,&n2)!=EOF)
    	{
		adlist[n1][++flag[n1]]=n2;
        	adlist[n2][++flag[n2]]=n1;
        	cd++;
    	}

fprintf(ad,"%d %d\n",max_label,cd);

for(i=1;i<=max_label;i++)
{
	fprintf(ad,"%d %d ",i,degree[i]);
	for(j=1;j<=degree[i];j++)
	{
		fprintf(ad,"%d ",adlist[i][j]);
	}
	fprintf(ad,"\n");
}


fclose(ad);
    	free(flag);
	free(degree);
}



void community_detection(float t)
{
    	int i,j,p,rear=-1,front=-1,top=0;
    	brok *bro;
	link * temp;
    	int z=0;
    	bro=(brok *)calloc(max_label+2,sizeof(brok));
    	int b=0,bLabel;
    	float reScore;
    	int maxi,cd;
	count_of_comm=0;
    	node start,neighbour;
    	clock_t begin=clock(),end;
    	while(front<rear || top>=0)
    	{
		if(front<rear)
        	{
            		start=Queue[++front];
                }
        	else
        	{
            		start=Broker[top--];
            		b++;//Broker node
			temp=(link*)malloc(sizeof(link));
       			temp->label=0;
			temp->mark=0;
			temp->test=0;
        		temp->ptr=NULL;
        		Overlap[start.label]=temp;
        	}

        	labels[start.label].traversed=1;
        	if(start.category==1)
        	{
            		bro[z].label=start.label;
            		bro[z].parent=start.parent;
            		z++;
        	}
            	for(i=1;i<=adlist[start.label][0];i++)
            	{
                	labels[adlist[start.label][i]].influenced=1;
            	}
            	for(i=1;i<=adlist[start.label][0];i++)
            	{
                	if(labels[adlist[start.label][i]].traversed==0)
                	{
                    		neighbour.parent=start.label;
                    		neighbour.label=adlist[start.label][i];
                    		neighbour.community=start.label;
		    		labels[neighbour.label].traversed=1;
                    		neighbour.score=calculate_score(neighbour.label);
                    		if(neighbour.score<t)
                    		{
                        		neighbour.category=1;//Broker
                       			labels[neighbour.label].label=neighbour.label;//broker
                        		labels[neighbour.label].category=1;
                        		Broker[++top]=neighbour;
                    		}
                    		else
                    		{
					if(labels[labels[start.label].label].mark1==0)
					{
						labels[labels[start.label].label].mark1=-1;
						count_of_comm++;
						Overlap[start.label]->mark=count_of_comm;
					}
                        		labels[neighbour.label].label=labels[start.label].label;
                        		labels[neighbour.label].category=2;
                        		neighbour.category=2;//Community node
                        		Queue[++rear]=neighbour;
					temp=(link*)malloc(sizeof(link));
       					temp->label=count_of_comm;
					temp->mark=0;
					temp->test=0;
        				temp->ptr=NULL;
        				Overlap[neighbour.label]=temp;
                    		}
                	}
		}
	}
	free(Queue);
    	free(Broker);
    	//printf("\nThe number of brokers are: %d",z);
    	//printf("\nThe number of brokers are: %d",z);
    	cd=0;
	//printf("\n\nNo. of COMMUNITIES count : %d\n",count_of_comm);
    	noOfComm=cd;
    	end=clock();
	//printf("\n\nTime taken = %f\n",(float)(end-begin)/CLOCKS_PER_SEC);
    	free(bro);
}
/*Calculates score*/

float calculate_score(int index)
{
    int i,c=0;
    for(i=1;i<=adlist[index][0];i++)
    {
        if(labels[adlist[index][i]].influenced==1)
            c++;
    }
    return ((float)c/adlist[index][0]);
}


//Calculates the number of edges of the a broker in a community


/*Covergence Algorithm*/
int t;
void convergence()
{
	clock_t begin,end;
	begin=clock();
    	int iter=5,i,j,no_of_comm,l;
    	link *temp;
    	char str[50];
	end=clock();
/* Placing marked nodes in communities to which maximum neighbours belong */
	int check_loop=1,vertex=0;
	while(check_loop==1)
	{
		check_loop=0;
		for(i=1;i<=max_label;i++)
		{
			if(labels[i].category==1)
			{
				vertex=i;
				maxCMT(vertex);
				if(labels[vertex].category==1)
					check_loop=1;
			}
		}
	}
}

/*Calculates the maximum label*/
void maxCMT(int n)
{
	int i,j,k=0;
	float max=0.0;
    	link *temp,*temp1,*temp2;
    	int *flag=(int *)calloc(count_of_comm+1,sizeof(int));
    	for(i=1;i<=adlist[n][0];i++)
    	{
		if(labels[adlist[n][i]].category!=1)
		{
  	    	temp=Overlap[adlist[n][i]];
        	while(temp!=NULL)
        	{

            		flag[temp->label]++;

            		temp=temp->ptr;
        	}
        	free(temp);
		}
    	}

    	for(i=1;i<=adlist[n][0];i++)
    	{
		if(labels[adlist[n][i]].category!=1)
		{
		temp=Overlap[adlist[n][i]];
		while(temp!=NULL)
		{
			if(flag[temp->label]!=-1)
			{
				if(((float)flag[temp->label])>max)
				{
					labels[n].category=3;
					max=flag[temp->label];
					temp1=Overlap[n];
					while(temp1!=NULL)
					{
						temp2=temp1;
						temp1=temp1->ptr;
						free(temp2);
					}
					temp2=(link*)malloc(sizeof(link));
					temp2->label=temp->label;
					temp2->mark=1;
					temp2->ptr=NULL;
					Overlap[n]=temp2;
				}
        			else if(((float)flag[temp->label])==max)
				{
                   			temp2=(link*)malloc(sizeof(link));
					temp2->label=temp->label;
					temp2->ptr=Overlap[n];
					temp2->mark=-1;
					Overlap[n]=temp2;
				}

             			flag[temp->label]=-1;
			}
			temp=temp->ptr;
        	}
		free(temp);
		}
    	}
    	free(flag);
}

/*Prints the communities*/
int print()
{
    	int i,j,l,no_of_comm=0,k,count;
    	link *temp,*temp1,*temp2;
	int cVal=1,index=0;
	FILE *fp;
	list1=(int *)calloc(count_of_comm+2,sizeof(int));
	int *arr=(int *)calloc(count_of_comm+2,sizeof(int));
	//FILE *fp123=fopen("Mapping.txt","w+");
	//fp=fopen("Membership.part","w+");
char mapping[50] = "Mapping";
char membership[50] = "Membership_";
strcat(mapping,filename);
strcat(membership,filename);
	FILE *fp123=fopen(mapping,"w+");
	fp=fopen(membership,"w+");
  	for(i=1;i<=max_label;i++)
    	{
		fprintf(fp,"%d",i);
        	temp=Overlap[i];
		if(temp->test!=0)
			index=temp->test;
        	while(temp!=NULL)
		{
			if(arr[temp->label]==0)
			{
            			arr[temp->label]=cVal;
				fprintf(fp123,"%d  %d  \n",temp->label,cVal);
				fprintf(fp," %d",arr[temp->label]);
	  			temp->label=cVal;
				list1[temp->label]++;
				cVal++;
			}
			else
            		{
				fprintf(fp," %d",arr[temp->label]);
                		temp->label=arr[temp->label];
				list1[temp->label]++;
            		}
        		temp=temp->ptr;
		}
		fprintf(fp," %d\n", -1);
        }

	fclose(fp);
	//char filename3[50],c1,c2;
	free(arr);
int *arr1=(int *)calloc(count_of_comm+2,sizeof(int));
char filename3[50]="",c1,c2;

	if(1)
	{
		strcat(filename3,filename);
		strcat(filename3,"_cover_i.txt");
		fp=fopen(filename3,"w+");
		fprintf(fp,"%d\n",count_of_comm);
        	for(i=1;i<=max_label;i++)
    		{
        		temp2=Overlap[i];
			while(temp2!=NULL)
			{
        			while(arr1[temp2->label]!=-1)
				{
					arr1[temp2->label]=-1;
        	    			for(j=i;j<=max_label;j++)
        	    			{
        	        			temp1=Overlap[j];
        	        			while(temp1!=NULL)
        	        			{
        	            				if(temp2->label==temp1->label)
        	            				{
        	                				fprintf(fp,"%d ",j);
        	            				}
        	            				temp1=temp1->ptr;
        	       				}
        	   			}
				fprintf(fp,"-1\n");
				}
        	   		temp2=temp2->ptr;
			}
		}
		fclose(fp);
	}

	free(arr1);
	//free(arr1);
    	return (cVal-1);
}



void generateNodes()
{
	link * temp,*temp1;
	int i,j,k,ct;
	int total=0;
	FILE *fpl,*fpl1;
	char name[50] ="louvain_input_";
	strcat(name,filename);
	fpl=fopen(name,"w+");
	strcat(name,filename);
	int index,index1;
        arrayL=(int**)calloc(no_of_comm1+2,sizeof(int*));
        for(i=1;i<=no_of_comm1+1;i++)
        {
            arrayL[i]=(int*)calloc((i+2),sizeof(int));
        }
        for(i=1;i<=max_label;i++)
      	{
		Overlap[i]->mark=-999;
                for(j=1;j<=adlist[i][0];j++)
               	{
			if(Overlap[adlist[i][j]]->mark!=-999)
			{
                          	temp=Overlap[i];
				temp1=Overlap[adlist[i][j]];
				while(temp!=NULL)
				{
					while(temp1!=NULL)
                             		{
						if(temp->label!=temp1->label)
						{
                                      			//if((temp->label)>(temp1->label) && arrayL[temp->label][temp1->label]==0)
							if((temp->label)>(temp1->label))
                                      			{
								if(arrayL[temp->label][temp1->label]==0)
									fprintf(fpl,"%d %d\n",temp->label,temp1->label);
                                          			arrayL[temp->label][temp1->label]++;
                                       			}
                                        		//else if((temp->label) < (temp1->label) && arrayL[temp1->label][temp->label]==0)
							else if((temp->label) < (temp1->label))
                                          		{
								if(arrayL[temp1->label][temp->label]==0)
									fprintf(fpl,"%d %d\n",temp->label,temp1->label);
                                       	       			arrayL[temp1->label][temp->label]++;
                                       	 		}
                                		}
						temp1=temp1->ptr;
					}
					temp=temp->ptr;
				}
			}
		}
	}

	/*fpl1=fopen("NewEdge.txt","w+");
	for(i=1;i<=no_of_comm1;i++)
	{
		for(j=i;j<=no_of_comm1;j++)

		{
			if((arrayL[j][i]!=0))
			{
				//fprintf(fpl,"%d %d\n",i,j);
				fprintf(fpl1,"%d %d %f\n",i,j,(float)arrayL[j][i]/(list1[i]*list1[j]));
				total++;
			}
		}
	}*/
	fclose(fpl);
	//fclose(fpl1);
	//printf("total No. of edges : %d\n",total);

	for(i=0;i<=no_of_comm1+1;i++)
		free(arrayL[i]);
	free(arrayL);
	free(list1);
}
