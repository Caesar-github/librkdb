#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <glib.h>

#include "json-c/json.h"

static sqlite3 *db;

static int callback(void *param, int argc, char **argv, char **azColName)
{
    int i;
    json_object *j_array = (json_object *)param;
    json_object *j_cfg = json_object_new_object();

    for (i = 0; i < argc; i++) {
        if (azColName[i][0] == 'i') {
            json_object_object_add(j_cfg, azColName[i], json_object_new_int(atoi(argv[i])));
        } else {
            json_object_object_add(j_cfg, azColName[i], json_object_new_string(argv[i] ? argv[i] : ""));
        }
    }
    json_object_array_add(j_array, j_cfg);

    return 0;
}

int rkdb_creat(char *table, char *col_para)
{
    int ret = -1;
    int rc;
    char *sql;
    char *zErrMsg = 0;

    sql = g_strdup_printf("CREATE TABLE %s(%s);", table, col_para);

    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        printf("created %s Table fail err: %s\n", table, zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        //printf("created %s Table successfully\n", table);
        ret = 0;
    }
    g_free(sql);

    return ret;
}

int rkdb_insert(char *table, char *cols, char *vals)
{
    int ret = -1;
    int rc;
    char *sql;
    char *zErrMsg = 0;

    sql = g_strdup_printf("INSERT INTO %s (%s) VALUES (%s);", table, cols, vals);
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        printf("%s Insert data fail err: %s\n", table, zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        //printf("%s Insert data successfully\n", table);
        ret = 0;
    }
    g_free(sql);

    return ret;
}

int rkdb_update(char *table, char *set, char *where)
{
    int ret = -1;
    int rc;
    char *sql;
    char *zErrMsg = 0;

    sql = g_strdup_printf("UPDATE %s SET %s WHERE %s;", table, set, where);
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        printf("%s update data fail err: %s\n", table, zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        //printf("%s update data successfully\n", table);
        ret = 0;
    }
    g_free(sql);

    return ret;
}

int rkdb_delete(char *table, char *where)
{
    int ret = -1;
    int rc;
    char *sql;
    char *zErrMsg = 0;

    if (where)
        sql = g_strdup_printf("DELETE from %s WHERE %s;", table, where);
    else
        sql = g_strdup_printf("DELETE from %s;", table);
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        printf("%s delete data fail err: %s\n", table, zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        //printf("%s delete data successfully\n", table);
        ret = 0;
    }
    g_free(sql);

    return ret;
}

json_object *rkdb_select(char *table, char *colname, char *where, char *order, char *limit)
{
    char *sql = NULL;
    char *tmp;
    int rc;
    char *zErrMsg = 0;
    json_object *j_array = json_object_new_array();

    if (colname)
        sql = g_strdup_printf("SELECT %s from %s", colname, table);
    else
        sql = g_strdup_printf("SELECT * from %s", table);

    if (where) {
        tmp = sql;
        sql = g_strdup_printf("%s WHERE %s", tmp, where);
        g_free(tmp);
    }

    if (order) {
        tmp = sql;
        sql = g_strdup_printf("%s ORDER BY %s", tmp, order);
        g_free(tmp);
    }

    if (limit) {
        tmp = sql;
        sql = g_strdup_printf("%s LIMIT %s", tmp, limit);
        g_free(tmp);
    }

    rc = sqlite3_exec(db, sql, callback, (void*)j_array, &zErrMsg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        //printf("Operation done successfully\n");
    }
    g_free(sql);
    //printf("%s %s\n", __func__, (char *)json_object_get_string(j_array));

    return j_array;
}

int rkdb_init(char *file)
{
    int rc;
    char *zErrMsg = 0;

    rc = sqlite3_open(file, &db);
    assert(!rc);

    return 0;
}

void rkdb_deinit(void)
{
    if (db)
        sqlite3_close(db);

    db = NULL;
}