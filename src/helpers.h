#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <libxml/parser.h>

xmlNodePtr find_next_node(xmlNodePtr cur, char *name);
int get_defn_int_val(xmlNodePtr defn, char *prop);
int get_defn_string(xmlNodePtr defn, char *prop, char *string);
int get_physical_part_number(xmlNodePtr tmp);
void string_remove_char(char *str, char garbage);
void string_replace_char(char *string, char from, char to);
void get_physical_part_pin_number(xmlNodePtr phy_part, int index, char *name);

#endif
