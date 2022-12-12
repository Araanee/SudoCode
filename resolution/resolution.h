#ifndef RESOLUTION_H
#define RESOLUTION_H

size_t GetPositionToRemove(int *poss, int x);

void GetPossibilities(int (*array)[9], int *poss, int i, int j);

int Solve(char *filename);

#endif