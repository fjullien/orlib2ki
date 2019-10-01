#ifndef __DRAW_H__
#define __DRAW_H__

void do_ellipse(int part_index, xmlNodePtr ellipse, FILE *fp);
void do_arc(int part_index, xmlNodePtr ellipse, FILE *fp);
void do_line(int part_index, xmlNodePtr line, FILE *fp);
void do_polygon(int part_index, xmlNodePtr poly, FILE *fp);
void do_text(int part_index, xmlNodePtr text, FILE *fp);
void do_user_prop(xmlNodePtr libpart, struct def compo, FILE *fp);
void do_rectangle(int part_index, xmlNodePtr rect, FILE *fp);
void do_pin(xmlNodePtr rect, struct pin *pin);
void do_physical(xmlNodePtr tmp, struct pin *pin, FILE *fp, int unit);

#endif
