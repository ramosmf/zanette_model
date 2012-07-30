//
//  network.h
//  DK-model
//
//  Created by Marlon Ramos on 18/06/12.
//  Copyright (c) 2012 PUC-Rio. All rights reserved.
//

#ifndef DK_model_network_h
#define DK_model_network_h

void pajek(char *str);
int neighbor(int v1, int v2);
int find_nbr(int v, int u);
void addLink(int i, int j);
void remove_link(int v1, int v2);
void destroyNetwork(void);

void SWN(double p);

#endif
