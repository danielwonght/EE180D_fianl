#ifndef DATA_COLLECTION_H
#define DATA_COLLECTION_H

void initTermios(int echo);
void resetTermios(void);
char getch_(int echo);
char getch(void);
char getche(void);
int kbhit(void);
double parse_tv(struct timeval *tv);
void data_collection(char *file_name);
void concatenate_string(char *original, char *add);

#endif

