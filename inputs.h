#ifndef SIETE_Y_MEDIO_INPUTS_H
#define SIETE_Y_MEDIO_INPUTS_H
#include <stdbool.h>

int getUserInputAsInt(char * userQuestion, int min, int max);

double getUserInputAsDouble(char *userQuestion, double min, double max);

bool getUserInputAsBool(char *userQuestion);

#endif //SIETE_Y_MEDIO_INPUTS_H