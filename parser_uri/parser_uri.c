
#include "parser_uri.h"

void parse_uri (char* str){
	static const char *ws = "?\t"; // Whitespace characters that separate tokens
	char *command = str;

	// Find the end of the first word
	str += strcspn(str, ws);
	if (*str) {
		*str++ = 0; // Terminate command
	}
	printf("Command: %s\n", command);

	while (*str) {
	// Skip leading whitespace
		str += strspn(str, ws);
		if (!*str) {
		  // That was actually trailing whitespace at the end of the string
		  break;
	}

	// Split at = sign
	char *option = str;
	str = strchr(str, '=');
	if (!str) {
		  fputs("Missing = after option!\n", stderr);
		  exit(EXIT_FAILURE);
	}
	*str++ = 0; // Terminate option

	// Parse the numeric argument
	char *valstr = str;
	double val = strtod(valstr, &str);
	if (valstr == str || !strchr(ws, *str)) {
		  fprintf(stderr, "Non-numeric argument to %s!\n", option);
		  exit(EXIT_FAILURE);
	}

	printf(" Option %s, value %f\n", option, val);
	}
}

