//
//  network.c
//  DK-model
//
//  Created by Marlon Ramos on 18/06/12.
//  Copyright (c) 2012 PUC-Rio. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "network.h"

int **nbr; // nbracency list
int *Z; // Vector that keep the vertices degree
int N, K; // Total number of vertex and edges

unsigned long int lim=-1, a=(sizeof(int)==4)?302875106592253:16807,  r=13976591; //lcg random number generator

void addLink(int i, int j);

void pajek(char *str){
    int m, n;
    FILE *PAJ;
    
    char string[1024];
    sprintf(string, "pajek-%s.net", str);
    PAJ = fopen(string, "w");
    
    fprintf(PAJ, "*Vertices %d\r\n", N);
    for (m=1;m<N+1;m++) fprintf (PAJ,"%d '%d'\r\n", m, m); //Nomeia os vertices
    
    fprintf (PAJ,"*Edges\r\n");
    for(m=0;m<N;m++){
        for(n=0;n<Z[m];n++){
            if((nbr[m][n]+1)>(m+1))
                fprintf(PAJ, "%d %d\r\n", m+1, nbr[m][n]+1);
        }
    }
    
    fclose(PAJ);
}

void addLink(int i, int j){
    /*  Add link between i and j */
    
    if(!(nbr[i] = (int *)realloc(nbr[i],(Z[i]+2) * sizeof(int))))
        printf("Not enough memory\n");
    else{
        nbr[i][Z[i]] = j;
        Z[i]++;  
    }
    if((nbr[j] = (int *)realloc(nbr[j], (Z[j]+2) * sizeof(int) ) ) == NULL)
        printf("Not enough memory\n");
    else{
        nbr[j][Z[j]] = i;
        Z[j]++;
    }
}

void SWN(double p){
    int i, j, v, s;
    
    nbr = (int **)realloc(NULL, N * sizeof(int *));
    for(i=0;i<N;i++)  nbr[i]=(int *)calloc(1, sizeof(int));
    Z = (int *)calloc(N, sizeof(int));

    
    for (j=1; j<=K/2; j++) {
        for (v=0; v<N; v++) {
            s=(v+j)%N;   
            addLink(v, s);    
        }
    }
    
    for (j=1; j<=K/2; j++) {
        for (v=0; v<N; v++) {
            s=(v+j)%N;
            r=r*a;
            if((double)r/lim<p){
                remove_link(v, s);  
                do {
                    r=r*a;
                    s=N*(double)r/lim;
                } while ((s==v)||(find_nbr(v, s)!=-1));
                addLink(v, s);
            }
        }
    }
   
}

void remove_link(int v1, int v2){
    int x;
    
    x=find_nbr(v1, v2);
    Z[v1]--;
    nbr[v1][x]=nbr[v1][Z[v1]];
    nbr[v1] = (int *)realloc(nbr[v1], (Z[v1]+1) * sizeof(int)); 
    
    x=find_nbr(v2, v1);
    Z[v2]--;
    nbr[v2][x]=nbr[v2][Z[v2]];
    nbr[v2] = (int *)realloc(nbr[v2], (Z[v2]+1) * sizeof(int)); 
    
}

int neighbor(int v1, int v2){
    int i;
    for(i=0;i<Z[v1];i++){
        if(nbr[v1][i]==v2) return 1;
    }
    return 0;    
}

int find_nbr(int v, int u){
    /* return i if u is the i-th neighbor of v and -1 otherwise*/
    int i; 
    
    for(i=0;i<Z[v];i++){
        if(u==nbr[v][i]) {return i;}
        
    }
    return -1;
}

void destroyNetwork(void){
    /* Free Memory */
    int i;
    free(Z);
    for(i=0;i<N;i++) free(nbr[i]); 
    free(nbr);
}



