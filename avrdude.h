#ifndef AVRDUDE_H
#define AVRDUDE_H

#include <geanyplugin.h>
#include <gtk/gtk.h>
#include <malloc.h>

FILE *avrdude_cmd(char *cmd);
int avrdude_get_avrs(char ***avrs, int *n_avrs);

#endif //AVRDUDE_H
