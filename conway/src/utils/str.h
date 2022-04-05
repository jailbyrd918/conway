/*
	DESCRIPTION:
		Handles string related utility functions
*/

#ifndef STR_H
#define STR_H


// >> checks if two given strings are matched
bool str_matched
(const char *str1, const char *str2);

// >> checks if given string is empty
bool str_empty
(const char *str);

// >> checks if given string contains the given substring
bool str_contains_substr
(const char *str, const char *substr);

// >> returns the index of the given substring inside the given string
int str_search_substr
(const char *str, const char *substr);

// >> returns a mutable copy of the given string
char *str_copy
(const char *str);

// >> concatenates the given string reference by the given source string
bool str_concatenate
(char **dest_str, const char *src_str);

// >> concatenates two given strings and returns a new one
char *str_concatenate_new
(const char *str1, const char *str2);

// >> combines multiple strings and returns a new one
// >>> specify number of strings to connect under [arg] <str_num>
char *str_combine_strs
(const int str_num, const char *str, ...);


#endif
