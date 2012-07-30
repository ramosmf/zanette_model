//
//  main.c
//  DK-model
//
//  Created by Marlon Ramos on 18/06/12.
//  Copyright (c) 2012 PUC-Rio. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "network.h"
#include "measures.h"

int **nbr; // Adjacency list
int *Z; // Vector that keep the vertices degree
int N, K; // Total number of vertex and edges  

int main (int argc, const char * argv[])
{
    int j, v, s, x, na, NA, Ninf, Nsuc, Nrec;
    double p, R_inf;
    int *state, *infec_list;
    char str[1024];
    FILE *outf;
    extern unsigned long int r, a, lim; 

    if(!(argc==4)){printf("error: invalid argument\nsintax: %s [lattice size] [mean degree] [sample size]\n", argv[0]); return 0;}

    N=atoi(argv[1]);
    K=atoi(argv[2]);;
    NA=atoi(argv[3]);;
    
    sprintf(str, "refractory-%d-%d-%d.dat", N, K, NA);
    outf=fopen(str, "w");
    
    state = (int *)calloc(N, sizeof(int));
    infec_list = (int *)calloc(N, sizeof(int));
    
    for (p=0.0; p<=1.0; p+=0.02) {
        
        R_inf=0;
        
        for (na=1; na<=NA; na++) {

            SWN(p); for(v=0; v<N; v++){state[v]=0;}    
            
            r=r*a; v=N*(double)r/lim;
            infec_list[0]=v; state[v]=1;
            Ninf=1; Nsuc=N-1; Nrec=0;
                     
            while (Ninf!=0) {
         
                r=r*a; x=Ninf*(double)r/lim; v=infec_list[x];
                r=r*a; j=Z[v]*(double)r/lim; s=nbr[v][j];
            
                if (state[s]==0) {
                    state[s]=1;
                    infec_list[Ninf]=s;
                    Ninf++; Nsuc--;
                }
                else{
                    state[v]=2;
                    Ninf--; Nrec++;
                    infec_list[x]=infec_list[Ninf];
                }
            }
            
            R_inf+=Nrec;
            destroyNetwork();
        }
        
        R_inf=(double)R_inf/(N*NA); 
        fprintf(outf, "%lf %lf\n", p, R_inf);
        
    }
    return 0;
}

