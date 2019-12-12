#ifndef __RKDB_H__
#define __RKDB_H__

#ifdef __cplusplus
extern "C" {
#endif

int rkdb_creat(char *table, char *col_para);
int rkdb_update(char *table, char *set, char *where);
int rkdb_insert(char *table, char *cols, char *vals);
int rkdb_delete(char *table, char *where);
json_object *rkdb_select(char *table, char *colname, char *where, char *order, char *limit);
int rkdb_init(char *file);
void rkdb_deinit(void);

#ifdef __cplusplus
}
#endif

#endif