#ifndef __ORLIB2KI_H__
#define __ORLIB2KI_H__

#define PI			3.14159
#define ELLIPSE_NB_SEGMENTS	40

struct def {
	char name[128];
	char refdes[128];
	char footprint[128];
	int draw_pinnumber;
	int draw_pinname;
	int unit_count;
	int units_locked;
	int nb_phys_part;
	int part_count;
	char value[128];
	int value_x;
	int value_y;
	int box_right_x;
	int box_right_y;
	int has_value;
	int val_rot;
};

struct field {
	int field_number;
	xmlChar *text;
	int x;
	int y;
	int dimension;
	char orientation[2];
	char visibility[2];
	char hjustify[2];
	char vjustify[2];
	char italic;
	char bold;
	xmlChar *name;
	int rotation;
};

struct pin {
	int x1, y1, x2, y2;
	char orientation[2];
	char shape[2];
	int size;
	int type;
	int is_clock;
	int is_dot;
	char name[128];
	int position;
};

#endif
