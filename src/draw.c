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
#include <math.h>
#include <libxml/parser.h>

#include "orlib2ki.h"
#include "helpers.h"

void do_ellipse(float grid_scale, int part_index, xmlNodePtr ellipse, FILE *fp)
{
	int x1, y1, x2, y2;
	xmlNodePtr defn;
	int fill_style;
	int rx, ry;
	int cx, cy;
	float step, theta;
	int last_x, last_y, x, y;

	ellipse = ellipse->xmlChildrenNode;
	defn = find_next_node(ellipse, "Defn");

	x1 = get_defn_int_val(defn, "x1");
	x2 = get_defn_int_val(defn, "x2");
	y1 = get_defn_int_val(defn, "y1");
	y2 = get_defn_int_val(defn, "y2");

	fill_style = get_defn_int_val(defn, "fillStyle");

	rx = (x2 - x1) / 2;
	ry = (y2 - y1) / 2;

	cx = (x1 + x2) / 2;
	cy = (y1 + y2) / 2;

	if (rx == ry) {
		rx = rx * grid_scale;
		cx = cx * grid_scale;
		cy = cy * -grid_scale;
		fprintf(fp, "C %d %d %d %d 1 4 %s\n", cx, cy, rx, part_index, fill_style == 1 ? "N" : "F");
	} else {
		step =  (2 * PI) / 40;

		last_x = (int)((float)(cx) + rx * cos(step));
		last_y = (int)((float)(cy) + ry * sin(step));
		for(theta = step; theta < (2 * PI); theta += step) {
			x = (int)((float)(cx) + rx * cos(theta));
			y = (int)((float)(cy) + ry * sin(theta));
			fprintf(fp, "P 2 %d 1 4 %d %d %d %d N\n", part_index, (int)(last_x * grid_scale), (int)(last_y * -grid_scale), (int)(x * grid_scale), (int)(y * -grid_scale));
			last_x = x;
			last_y = y;
		}

		fprintf(fp, "P 2 %d 1 4 %d %d %d %d N\n", part_index, (int)(last_x * grid_scale), (int)(last_y * -grid_scale), (int)((int)((float)(cx) + rx * cos(step)) * grid_scale), (int)((int)((float)(cy) + ry * sin(step)) * -grid_scale));
	}
}

void do_arc(float grid_scale, int part_index, xmlNodePtr ellipse, FILE *fp)
{
	int x1, y1, x2, y2, sx, sy, ex, ey;
	xmlNodePtr defn;
	int rx, ry;
	int cx, cy;
	float step, theta;
	float start_angle;
	float end_angle;
	int last_x, last_y, x, y;

	ellipse = ellipse->xmlChildrenNode;
	defn = find_next_node(ellipse, "Defn");

	x1 = get_defn_int_val(defn, "x1") * grid_scale;
	x2 = get_defn_int_val(defn, "x2") * grid_scale;
	y1 = get_defn_int_val(defn, "y1") * -grid_scale;
	y2 = get_defn_int_val(defn, "y2") * -grid_scale;
	sx = get_defn_int_val(defn, "startX") * grid_scale;
	sy = get_defn_int_val(defn, "startY") * -grid_scale;
	ex = get_defn_int_val(defn, "endX") * grid_scale;
	ey = get_defn_int_val(defn, "endY") * -grid_scale;

	rx = abs((x2 - x1) / 2);
	ry = abs((y2 - y1) / 2);

	cx = (x1 + x2) / 2;
	cy = (y1 + y2) / 2;

	start_angle = atan2(sy - cy, sx - cx);
	end_angle = atan2(ey - cy, ex - cx);

	if (end_angle <= 0)
		end_angle = (2 * PI) + end_angle;

	if (start_angle < 0)
		start_angle = (2 * PI) + start_angle;

	if (end_angle < start_angle)
		end_angle = end_angle + (2 * PI);

	step = fabsf(end_angle - start_angle) / (float)ELLIPSE_NB_SEGMENTS;

	last_x = sx;
	last_y = sy;
	for(theta = start_angle; theta < end_angle; theta += step) {
		x = (int)((float)(cx) + rx * cos(theta));
		y = (int)((float)(cy) + ry * sin(theta));
		fprintf(fp, "P 2 %d 1 4 %d %d %d %d N\n", part_index, last_x, last_y, x, y);
		last_x = x;
		last_y = y;
	}

	fprintf(fp, "P 2 %d 1 4 %d %d %d %d N\n", part_index, last_x, last_y, ex, ey);
}


void do_line(float grid_scale, int part_index, xmlNodePtr line, FILE *fp)
{
	int x1, y1, x2, y2;
	xmlNodePtr defn;

	line = line->xmlChildrenNode;
	defn = find_next_node(line, "Defn");

	x1 = get_defn_int_val(defn, "x1") * grid_scale;
	x2 = get_defn_int_val(defn, "x2") * grid_scale;
	y1 = get_defn_int_val(defn, "y1") * -grid_scale;
	y2 = get_defn_int_val(defn, "y2") * -grid_scale;

	fprintf(fp, "P 2 %d 1 4 %d %d %d %d N\n", part_index, x1, y1, x2, y2);
}

void do_polygon(float grid_scale, int part_index, xmlNodePtr poly, FILE *fp)
{
	int x, y, next_x, next_y;
	xmlNodePtr point, defn;

	point = poly->xmlChildrenNode;
	point = find_next_node(point, "PolygonPoint");

	defn = point->xmlChildrenNode;
	defn = find_next_node(defn, "Defn");
	x = get_defn_int_val(defn, "x") * grid_scale;
	y = get_defn_int_val(defn, "y") * -grid_scale;

	while (point) {
		defn = point->xmlChildrenNode;
		defn = find_next_node(defn, "Defn");
		next_x = get_defn_int_val(defn, "x") * grid_scale;
		next_y = get_defn_int_val(defn, "y") * -grid_scale;
		fprintf(fp, "P 2 %d 1 4 %d %d %d %d N\n", part_index, x, y, next_x, next_y);
		x = next_x;
		y = next_y;
		point = find_next_node(point->next, "PolygonPoint");
	}
}

void do_text(float text_scale, float grid_scale, int part_index, xmlNodePtr text, FILE *fp)
{
	int escapement, italic, weight;
	int x, y, height;
	char name[128];
	xmlNodePtr tmp;
	char *ptr;

	text = text->xmlChildrenNode;
	tmp = find_next_node(text, "Defn");

	x = get_defn_int_val(tmp, "locX") * grid_scale;
	y = get_defn_int_val(tmp, "locY") * -grid_scale;
	get_defn_string(tmp, "name", name);

	tmp = find_next_node(text, "TextFont");
	tmp = tmp->xmlChildrenNode;
	tmp = find_next_node(tmp, "Defn");

	escapement  = get_defn_int_val(tmp, "escapement");
	italic      = get_defn_int_val(tmp, "italic");
	weight      = get_defn_int_val(tmp, "weight");
	height      = get_defn_int_val(tmp, "height") * text_scale;

	ptr = strtok(name, "\r\n");

	while(ptr != NULL) {
		fprintf(fp, "T %d %d %d %d 0 %d 1 \"%s\" %s %d C C\n", escapement, x, y, -height, part_index, ptr, italic ? "Italic" : "Normal", weight == 700 ? 1 : 0);
		if (escapement == 0)
			y = y - (height * 2);
		if (escapement == 900)
			x = x + (height * 2);
		if (escapement == 1800)
			y = y + (height * 2);
		if (escapement == 2700)
			x = x - (height * 2);
		ptr = strtok(NULL, "\r\n");
	}
}

void do_user_prop(int text_size, xmlNodePtr libpart, struct def compo, FILE *fp)
{
	xmlNodePtr tmp, user_prop;
	char name[128];
	char val[128];
	int i = 5;
	int x, y;

	tmp = libpart->xmlChildrenNode;
	tmp = find_next_node(tmp, "NormalView");
	tmp = tmp->xmlChildrenNode;

	user_prop = find_next_node(tmp, "SymbolUserProp");

	x = compo.box_right_x;
	y = compo.box_right_y - text_size - 10;
	y = y - text_size - 10;

	while (user_prop) {
		tmp = user_prop->xmlChildrenNode;
		tmp = find_next_node(tmp, "Defn");

		get_defn_string(tmp, "name", name);
		get_defn_string(tmp, "val", val);
		user_prop = find_next_node(user_prop->next, "SymbolUserProp");
		fprintf(fp, "F%d \"%s\" %d %d %d H I L CNN \"%s\"\n", i++, val, x, y, text_size, name);
		y = y - text_size - 10;
	}
}

void do_rectangle(float grid_scale, int part_index, xmlNodePtr rect, FILE *fp)
{
	int x1, y1, x2, y2, fill_style_int;
	char fill_style[2];
	xmlNodePtr defn;

	defn = rect->xmlChildrenNode;
	defn = find_next_node(defn, "Defn");

	x1 = get_defn_int_val(defn, "x1") * grid_scale;
	x2 = get_defn_int_val(defn, "x2") * grid_scale;
	y1 = get_defn_int_val(defn, "y1") * -grid_scale;
	y2 = get_defn_int_val(defn, "y2") * -grid_scale;
	fill_style_int = get_defn_int_val(defn, "fillStyle");

	if (fill_style_int == 0)
		strcpy(fill_style, "F");
	else if (fill_style_int == 2)
		strcpy(fill_style, "f");
	else
		strcpy(fill_style, "N");

	fprintf(fp, "S %d %d %d %d %d 1 4 %s\n", x1, y1, x2, y2, part_index, fill_style);
}

void do_pin(float grid_scale, xmlNodePtr rect, struct pin *pin)
{
	xmlNodePtr defn, tmp;
	char temp[128];

	defn = rect->xmlChildrenNode;
	defn = find_next_node(defn, "Defn");

	pin->type = get_defn_int_val(defn, "type");

	pin->x1 = get_defn_int_val(defn, "hotptX") * grid_scale;
	pin->y1 = get_defn_int_val(defn, "hotptY") * -grid_scale;
	pin->x2 = get_defn_int_val(defn, "startX") * grid_scale;
	pin->y2 = get_defn_int_val(defn, "startY") * -grid_scale;
	pin->position = get_defn_int_val(defn, "position");

	get_defn_string(defn, "name", pin->name);
	string_replace_char(pin->name, ' ', '_');

	if (strchr(pin->name, '\\')) {
		string_remove_char(pin->name, '\\');
		strcpy(&temp[1], pin->name);
		temp[0] = '~';
		strcpy(pin->name, temp);
	}

	if ((pin->x1 - pin->x2) > 0) {
		strcpy(pin->orientation, "L");
		pin->size = abs(pin->x2 - pin->x1);
	} else if ((pin->x1 - pin->x2) < 0) {
		strcpy(pin->orientation, "R");
		pin->size = abs(pin->x2 - pin->x1);
	} else if ((pin->y1 - pin->y2) > 0) {
		strcpy(pin->orientation, "D");
		pin->size = abs(pin->y2 - pin->y1);
	} else if ((pin->y1 - pin->y2) < 0) {
		strcpy(pin->orientation, "U");
		pin->size = abs(pin->y2 - pin->y1);
	} else if (((pin->x1 - pin->x2) == 0) && ((pin->y1 - pin->y2) == 0)) {
		strcpy(pin->orientation, "R");
		strcpy(pin->shape, "X");
		pin->size = 0;
	}

	tmp = find_next_node(defn, "IsClock");
	tmp = tmp->xmlChildrenNode;
	pin->is_clock = get_defn_int_val(tmp, "val");

	tmp = find_next_node(defn, "IsDot");
	tmp = tmp->xmlChildrenNode;
	pin->is_dot = get_defn_int_val(tmp, "val");
}

void do_physical(int text_size, xmlNodePtr tmp, struct pin *pin, FILE *fp, int unit)
{
	const char type[9] = {'I', 'B', 'O', 'C', 'P', 'T', 'E', 'W', 'U'};
	char name[128];

	tmp = find_next_node(tmp->next, "PhysicalPart");
	while (tmp) {
		get_physical_part_pin_number(tmp, pin->position, name);
		fprintf(fp, "X %s %s %d %d %d %s %d %d %d 0 %c\n", pin->name, name[0] == 0 ? pin->name : name, pin->x1, pin->y1, pin->size, pin->orientation, text_size, text_size, unit++, type[pin->type]);
		tmp = find_next_node(tmp->next, "PhysicalPart");
	}
}
