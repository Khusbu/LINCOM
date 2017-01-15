#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int **array;
int **array1;
int *community;
float **v_comm;
double func( float a,float b);
double funcb( int comm , int node);

int vertices,m;
int main(int argc , char ** argv)
{
	char filename[50]="", filename1[50]="";
	int i,t,deg,node,comm=1,num,k,k1,j,tt,flag=0,a,b;
	strcat(filename,argv[1]); // adjlist.txt
	strcat(filename1,argv[2]); // final cover
	float j1;
	double part11=0.0,part22=0.0;
	double alpha1=0.0,alpha2=0.0,beta=0.0,part1=0.0,part2=0.0,final=0.0,final1=0.0,part21=0.0;
	FILE *fp;
	//fp=fopen("Adjlist.txt","r");
	fp=fopen(filename,"r");
if(fp==NULL)
{
printf("%s\n",filename);
printf("does not exist");
exit(0);
}
	fscanf(fp,"%d %d ",&vertices,&m);
	array=(int**)calloc(vertices+1,sizeof(int*));
	while(fscanf(fp,"%d ",&node)!=EOF)
        {
		fscanf(fp,"%d ",&deg);
		array[node]=(int*)calloc(deg+1,sizeof(int));
		array[node][0]=deg;
		//flag=0;
		for(i=1;i<=deg;i++)
                {
                        fscanf(fp,"%d ",&t);
                        array[node][i]=t;
                }
	}
int countl=0,test=0;
	fclose(fp);
	int *counter=(int *)calloc(vertices+1,sizeof(int));
	//fp=fopen("epi_cover.txt","r");
	fp=fopen(filename1,"r");
	fscanf(fp,"%d ",&num);
	v_comm=(float**)calloc(vertices+1,sizeof(float*));
        for(i=0;i<=vertices;i++)
                v_comm[i]=(float*)calloc(num+1,sizeof(float));
	community=(int*)calloc(num+1,sizeof(int));
	while(fscanf(fp,"%d",&i)!=EOF)
	{
		if(i!=-1)
		{
			v_comm[i][comm]=1.0;
			community[comm]++;
			counter[i]++;
			countl++;
		}
		else{
			//printf("%d\n",countl);
			if(countl<=3)
				test++;
			countl=0;
			comm++;}
	}
	fclose(fp);
	//printf("Test %d\n",test);
	for(i=1;i<=vertices;i++)
	{
		for(j=1;j<=num;j++)
		{
			if(i==1)
				v_comm[j][0]=community[j];
			if(v_comm[i][j]==1.0)
				v_comm[i][j]=(float)1/(counter[i]);
		}
	}

	free(counter);

	free(community);

	int *comm1;
	double *mark=(double*)calloc(vertices+1,sizeof(double));
	double partt=0.0,mark1=1.0,mark2=0.0;
	double tot_beta=1.0,tot=0.0,tot1=0.0;
	int kij=0;
	float alpha_val,alpha2_val;int jk=0;
	int index;int temp=0;double betav=0.0;int alpa=0;
	for(k=1;k<=num;k++)
	{
		temp=0;
		//printf("Community %d\n",k);
		part1=0.0,part2=0.0;
		for(j=0;j<=vertices;j++)
		{
			mark[j]=-1;
		}
		jk=0;
		comm1=(int*)calloc((int)v_comm[k][0],sizeof(int));

			for(j=1;j<=vertices;j++)
			{

				if(v_comm[j][k]!=0){
					comm1[jk]=j;jk++;}
				alpha2_val=v_comm[j][k];
				if(mark[j]==-1.0)
				{
					if(alpha2_val!=0)
						mark[j]=funcb(k,j);
					else
						mark[j]=0;
					for(index=1;index<=vertices;index++)
					{
						if(v_comm[index][k]==alpha2_val)
							mark[index]=mark[j];
					}
				}
			}

			for(i=1;i<=vertices;i++)
			{
				for(j=1;j<=array[i][0];j++)
				{
					if(i<array[i][j])
					{
						part1+=(func(v_comm[i][k],v_comm[array[i][j]][k]));
					}
				}
			}
			for(i=0;i<(int)(v_comm[k][0]);i++)
			{
				for(j=0;j<(int)(v_comm[k][0]);j++)
				{
					part2+=((double)array[comm1[i]][0]*(double)array[comm1[j]][0]*mark[comm1[i]]*mark[comm1[j]]);
				}
			}

		free(comm1);
		part2=part2/4;
		part2=part2/(double)m;
		final+=((part1/(double)(m))-(part2/(double)m));
		//printf("comm --------> %d %lf\n",k,(part1/(double)(m))-(part2/(double)m));
		//printf("Community %d / %d : %lf\n",k,num,final);
	}

	free(mark);
	printf("Modularity is: %lf\n",final);
	printf("Number of vertices: %d\n",vertices);
	for(i=0;i<=vertices;i++)
                free(array[i]);
        free(array);

	for(i=0;i<=vertices;i++)
                free(v_comm[i]);
        free(v_comm);

}

double func( float a, float b)
{
	double f1=(60*a)-30;
	double f2=(60*b)-30;
	f1=exp(-(f1));
	f2=exp(-(f2));
	f1+=1;
	f2+=1;
	double l=(double)1/(f1*f2);
	return(l);
}
/*
double funcb( int comm , int node)
{
	int j;
	double beta=0.0;
	float alpha2,alpha1;
	alpha1=v_comm[node][comm];
	for(j=1;j<=array[node][0];j++)
		{
			alpha2=v_comm[array[node][j]][comm];
			beta+=func(alpha1,alpha2);
		}
	beta=beta/(double)vertices;
	return beta;
}*/
double funcb( int comm , int node)
{
	int j;
	double beta=0.0;
	float alpha2,alpha1;
	alpha1=v_comm[node][comm];
	for(j=1;j<=vertices;j++)
	{
		alpha2=v_comm[j][comm];
		beta+=func(alpha1,alpha2);

	}
	beta=beta/(double)(vertices);

	return beta;
}
