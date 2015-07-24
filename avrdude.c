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

int avrdude_get_avrs(char ***avrs, int *n_avrs)
{
	char **result;
	char avrbuffer[80][10];
	int n = 0, c = 0;
	int i, j;
	int active_zone = FALSE;
	char buffer[120];
	FILE *output = avrdude_cmd("-c 01234");
	while (fgets(buffer, 120, output) != NULL)
	{
		if (!strcmp(buffer, "Valid programmers are:\n"))
			active_zone = TRUE;
		else if (!strcmp(buffer, "\n"))
			active_zone = FALSE;
		else if (active_zone)
		{
			if (c >= 9)
			{
				char **old = result;
				result = malloc((n + c) * sizeof(char*));
				memcpy(result, old, n);
				memcpy(result + n, avrbuffer, c);
				n = n + c;
				//for (i = 0; i < n; i++)
				//	free(old[i]);
				//TODO: FREE!!!
				c = 0;
			}
			i = 0;
			while (buffer[i] == ' ')
				i++;
			j = i;
			while (buffer[j] != '\0' && buffer[j] != ' ' && buffer[j] != '=')
				j++;
			strncpy(avrbuffer[c], buffer + i, j - i);
			avrbuffer[c][j - i] = '\0';
			//println_to_terminal(ispbuffer[c], FALSE);
			c++;
		}
	}
	char **old = result;
	result = malloc((n + c) * sizeof(char*));
	memcpy(result, old, n);
	memcpy(result + n, avrbuffer, c);
	n = n + c;
	
	*n_avrs = 1;
	
	//for (i = 0; i < *n_avrs; i++)
	//	println_to_terminal(*avrs[i]);
	//free(avrs);
	*avrs = result;	
}

int avrdude_get_isps(char **dest)
{
	
}
