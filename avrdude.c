#include "avrdude.h"

FILE *avrdude_cmd(char *cmd)
{
	char *command = NULL;
	asprintf(&command, "%s %s", "avrdude", cmd);
	println_to_terminal(command, TRUE);
	asprintf(&command, "%s 2>&1", command);
	FILE *output = popen(command, "r");
	free(command);
	return output;
}

int get_avrs_or_isps(StrList *list, int *number, char * cmd, char *startseq)
{
	int i, j;
	int active_zone = FALSE;
	char buffer[120];
	FILE *output = avrdude_cmd(cmd);
	while (fgets(buffer, 120, output) != NULL)
	{
		if (!strcmp(buffer, startseq))
			active_zone = TRUE;
		else if (!strcmp(buffer, "\n"))
			active_zone = FALSE;
		else if (active_zone)
		{
			i = 0;
			while (buffer[i] == ' ')
				i++;
			j = i;
			while (buffer[j] != '\0' && buffer[j] != ' ' && buffer[j] != '=')
				j++;
			char *entry = malloc((j - i + 1) * sizeof(char));
			strncpy(entry, buffer + i, j - i);
			entry[j - i] = '\0';
			strlist_insert(list, entry);
			*number++;
		}
	}
}

int avrdude_get_avrs(StrList *avrs, int *n_avrs)
{
	return get_avrs_or_isps(avrs, n_avrs, "-c avrisp", "Valid parts are:\n");
}

int avrdude_get_isps(StrList *isps, int *n_isps)
{
	return get_avrs_or_isps(isps, n_isps, "-c 01234", "Valid programmers are:\n");
}
