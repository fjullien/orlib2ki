/* Copyright 2019 Franck Jullien
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>

#include "orlib2ki.h"

xmlNodePtr find_next_node(xmlNodePtr cur, char *name)
{
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)name)))
			return cur;
		cur = cur->next;
	}

	return NULL;
}

int get_defn_int_val(xmlNodePtr defn, char *prop)
{
	xmlChar *val;
	int ret;

	val = xmlGetProp(defn, (const xmlChar *)prop);
	ret = atoi((char *)val);
	xmlFree(val);

	return ret;
}


int get_defn_string(xmlNodePtr defn, char *prop, char *string)
{
	xmlChar *val;

	val = xmlGetProp(defn, (const xmlChar *)prop);
	strncpy(string, (char *)val, 128);
	xmlFree(val);

	return 0;
}

int get_physical_part_number(xmlNodePtr tmp)
{
	int i = 0;
	while (tmp) {
		tmp = find_next_node(tmp, "PhysicalPart");
		if (tmp)
			i++;
		tmp = tmp->next;
	}

	return i;
}

void string_remove_char(char *str, char garbage)
{
	char *src, *dst;
	for (src = dst = str; *src != '\0'; src++) {
		*dst = *src;
		if (*dst != garbage)
			dst++;
	}
	*dst = '\0';
}

void string_replace_char(char *string, char from, char to)
{
	while (*string++) {
		if (*string == from)
			*string = to;
	}
}

void get_physical_part_pin_number(xmlNodePtr phy_part, int index, char *name)
{
	xmlNodePtr tmp;
	int i;

	tmp = phy_part->xmlChildrenNode;
	for (i = 0; i < (index + 1); i++) {
		tmp = find_next_node(tmp->next, "PinNumber");
	}

	get_defn_string(tmp->xmlChildrenNode, "number", name);
}
