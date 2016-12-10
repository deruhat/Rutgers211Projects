char * strdup (const char *s) {
    char *d = malloc (strlen (s) + 1);
    if (d == NULL) return NULL;
    strcpy (d,s);
    return d;
}

char * substr(const char *input, char *output, size_t start, size_t length) {
	strncpy(output, input + start, length);
	output[length] = '\0';
	return output;
}