#ifndef AVRDUDE_H
#define AVRDUDE_H

#include <geanyplugin.h>
#include <gtk/gtk.h>
#include <malloc.h>
#include <string.h>
#include "strlist.h"

FILE *avrdude_cmd(char *cmd);
int avrdude_get_avrs(StrList *avrs, int *n_avrs);
int avrdude_get_isps(StrList *isps, int *n_isps);

#endif //AVRDUDE_H
