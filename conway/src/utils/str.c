#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#include "str.h"


bool str_matched
(const char *str1, const char *str2)
{
	if (!str1 && !str2)	return true;
	if (!str1 || !str2)	return false;

	return (strcmp(str1, str2) == 0);
}

bool str_empty
(const char *str)
{
	return (str != NULL) && (str_matched(str, "") || strlen(str) == 0);
}

bool str_contains_substr
(const char *str, const char *substr)
{
	if (!str || !substr)	return false;

	return (strstr(str, substr) != NULL);
}

int str_search_substr
(const char *str, const char *substr)
{
	if (!str || !substr || !str_contains_substr(str, substr))
		return -1;

	int	foundidx = -1,
		curridx = 0;

	while (str[curridx++] != '\0') {
		if (str[curridx] == substr[0]) {
			foundidx = curridx;
			size_t	tempidx = curridx;

			char	cmpstr[256];
			size_t	cmpidx = 0;

			size_t	cmplen = 0;
			while (cmplen++ < strlen(substr) && isalnum(str[tempidx]))
				cmpstr[cmpidx++] = str[tempidx++];
			cmpstr[cmpidx] = '\0';

			if (str_matched(cmpstr, substr))
				return foundidx;
		}
	}

	return -1;
}

char *str_copy
(const char *str)
{
	if (!str)	return false;

	char *deststr = malloc(strlen(str) + 1);
	size_t i = 0;
	for (; str[i] != '\0'; ++i)
		deststr[i] = str[i];
	deststr[i] = '\0';

	return deststr;
}

bool str_concatenate(char **dest_str, const char *src_str)
{
	if (!(*dest_str) || !src_str)
		return false;

	size_t	startidx = strlen((*dest_str));

	size_t	newsize = strlen((*dest_str)) + strlen(src_str) + 1;
	char	*newmem = realloc((*dest_str), newsize);
	if (!newmem)	return false;
	(*dest_str) = newmem;

	size_t	i = startidx, j = 0;
	for (; src_str[j] != '\0'; ++i)
		(*dest_str)[i] = src_str[j++];
	(*dest_str)[i] = '\0';

	return true;
}

char *str_concatenate_new(const char *str1, const char *str2)
{
	if (!str1 && !str2)
		return NULL;
	else if (!str1 && str2)
		return str_copy(str2);
	else if (str1 && !str2)
		return str_copy(str1);

	char *finalstr = malloc(strlen(str1) + strlen(str2) + 1);

	size_t i = 0, j = 0;;
	for (; str1[i] != '\0'; ++i)
		finalstr[j++] = str1[i];
	i = 0;
	for (; str2[i] != '\0'; ++i)
		finalstr[j++] = str2[i];
	finalstr[j] = '\0';

	return finalstr;
}

char *str_combine_strs
(const int str_num, const char *str, ...)
{
	va_list	ls;
	int	i = 0;
	char	*finalstr = NULL;

	va_start(ls, str_num);

	for (i = 0; i < str_num; ++i) {
		const char *tempstr = va_arg(ls, const char *);

		if (!finalstr)
			finalstr = str_copy(tempstr);
		else
			finalstr = str_concatenate_new(finalstr, tempstr);
	}

	va_end(ls);
	return finalstr;
}



