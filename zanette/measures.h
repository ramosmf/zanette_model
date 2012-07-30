//
//  measures.h
//  DK-model
//
//  Created by Marlon Ramos on 18/06/12.
//  Copyright (c) 2012 PUC-Rio. All rights reserved.
//

#ifndef DK_model_measures_h
#define DK_model_measures_h

double count_multiedge(void);
double clustering_coeficient(void);
double average_path_lenght(void);

void initBfs(void);
void endBfs(void);
void bfs(int s);
void bfs2(int s);

int init_queue(void);
void enqueue(int x);
int dequeue(void);

#endif
