#ifndef __DRAW_H__
#define __DRAW_H__

void do_ellipse(float grid_scale, int part_index, xmlNodePtr ellipse, FILE *fp);
void do_arc(float grid_scale, int part_index, xmlNodePtr ellipse, FILE *fp);
void do_line(float grid_scale, int part_index, xmlNodePtr line, FILE *fp);
void do_polygon(float grid_scale, int part_index, xmlNodePtr poly, FILE *fp);
void do_text(float text_scale, float grid_scale, int part_index, xmlNodePtr text, FILE *fp);
void do_user_prop(int text_size, xmlNodePtr libpart, struct def compo, FILE *fp);
void do_rectangle(float grid_scale, int part_index, xmlNodePtr rect, FILE *fp);
void do_pin(float grid_scale, xmlNodePtr rect, struct pin *pin);
void do_physical(xmlNodePtr tmp, struct pin *pin, FILE *fp, int unit);

#endif
