//
//  measures.c
//  DK-model
//
//  Created by Marlon Ramos on 18/06/12.
//  Copyright (c) 2012 PUC-Rio. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "network.h"
#include "measures.h"
#include <limits.h>

int **nbr; // Adjacency list
int *Z; // Vector that keep the vertices degree
int N; // Total number of vertex and edges

int *Q;
int Q_tail, Q_head;
int *distance;
int init_queue();
void enqueue(int x);
int dequeue();
void bfs(int s);
void bfs2(int s);

int B; //number of burned vertex
int *burn;//0 if burn[i] not burned; 1 otherwise
int NC; //number of clusters
int *W; //W[i] is the size of cluster i
int *burn_order; //burn_order[i]=v if v was the ith burned vertex

double clustering_coeficient(void){
    int e=0;
    int s, v, vv;
    double E=0;
    
    for(s=0;s<N;s++){
        for(v=0;v<Z[s];v++){
            for(vv=v; vv<Z[s]; vv++){
                e+=neighbor(nbr[s][v], nbr[s][vv]);
            }
        }
        E+=(Z[s]*(Z[s]-1));
    }
    
    return (double)2*e/E;
}

double count_multiedge(void){
    int i, j, k, x;
    int rep=0;
    
    for(i=0;i<N;i++){
        for (j=0; j<Z[i]; j++) {
            x=nbr[i][j];
            for (k=0; k<Z[i]; k++) {
                if (x==nbr[i][k]) {
                    rep++;
                }
            }           
            rep--;
        }
    }
    return (double)rep/4;
}

double average_path_lenght(void){
    int i, e, n=0, d=0;
    B=NC=0;
    
    burn=(int *)calloc(N, sizeof(int));  
    burn_order=(int *)calloc(N, sizeof(int));
    W=(int *)calloc(N, sizeof(int));
    
    for (i=0; i<N; i++) {
        
        if(burn[i]==0){
            bfs2(i);
            NC++;
        }
        else bfs(i);
        
        for(e=0;e<N;e++){
            if(distance[e]!=INT_MAX) d+=distance[e]; 
        }   
    
    }
    
    for(i=0; i<NC; i++) n+=W[i]*(W[i]-1);
    
    free(W);
    free(burn);
    free(burn_order);
    
    return (double)d/n;
}

void bfs(int s){
    int u, v;
    
    for(u=0;u<N;u++) distance[u]=INT_MAX;
    
    distance[s]=0;
    init_queue();
    enqueue(s);
    
    while(Q_tail!=Q_head){
        u=dequeue();
        for(v=0;v<Z[u];v++){  
            if(distance[nbr[u][v]]==INT_MAX){
                distance[nbr[u][v]]=distance[u]+1;
                enqueue(nbr[u][v]);
            }
        }
    }
}

void bfs2(int s){
    
    int u, v;
    for(u=0;u<N;u++) distance[u]=INT_MAX;
    
    distance[s]=0;
    init_queue();
    enqueue(s); 
    
    burn_order[B]=s; B++; W[NC]++; burn[s]=1; 
    
    while(Q_tail!=Q_head){
        u=dequeue();
        for(v=0;v<Z[u];v++){  
            if(distance[nbr[u][v]]==INT_MAX){
                distance[nbr[u][v]]=distance[u]+1;
                enqueue(nbr[u][v]); 
                burn_order[B]=nbr[u][v]; B++; W[NC]++; burn[nbr[u][v]]=1; 
            }
        }
    }
}


int init_queue(void){
    Q_tail = Q_head = 0;
    return 0;
}

void enqueue(int x){
    Q[Q_tail]=x;
    Q_tail=(Q_tail+1)%(N-1);
}

int dequeue(void){
    int x=Q[Q_head];
    Q_head=(Q_head+1)%(N-1);
    return x;
}

void initBfs(void){
    distance=(int *)calloc(N, sizeof(int));
    Q=(int *)calloc(N, sizeof(int));
}

void endBfs(void){
    free(distance);
    free(Q);
    free(W);    
}

