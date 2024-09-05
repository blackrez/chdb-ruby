#include <ruby.h>
#include "chdb.h"

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

static VALUE rb_chdb_query_stable(int argc, VALUE *argv, VALUE self) {
    char **args = malloc(sizeof(char *) * argc);
    int i;

    for (i = 0; i < argc; i++) {
        args[i] = StringValueCStr(argv[i]);
    }

    struct local_result *result = query_stable(argc, args);
    free(args);

    VALUE rb_result = rb_struct_new(rb_struct_define("LocalResult",
                                                    "buf", "len", "elapsed",
                                                    "rows_read", "bytes_read", NULL),
                                    rb_str_new(result->buf, result->len),
                                    SIZET2NUM(result->len),
                                    DBL2NUM(result->elapsed),
                                    ULL2NUM(result->rows_read),
                                    ULL2NUM(result->bytes_read));

    free_result(result);
    return rb_result;
}

static VALUE rb_chdb_query_stable_v2(int argc, VALUE *argv, VALUE self) {
    char **args = malloc(sizeof(char *) * argc);
    int i;

    for (i = 0; i < argc; i++) {
        args[i] = StringValueCStr(argv[i]);
    }

    struct local_result_v2 *result = query_stable_v2(argc, args);
    free(args);

    VALUE rb_result = rb_struct_new(rb_struct_define("LocalResultV2",
                                                    "buf", "len", "elapsed",
                                                    "rows_read", "bytes_read",
                                                    "error_message", NULL),
                                    rb_str_new(result->buf, result->len),
                                    SIZET2NUM(result->len),
                                    DBL2NUM(result->elapsed),
                                    ULL2NUM(result->rows_read),
                                    ULL2NUM(result->bytes_read),
                                    result->error_message ? rb_str_new_cstr(result->error_message) : Qnil);

    free_result_v2(result);
    return rb_result;
}

void Init_chdb_ext() {
    VALUE mCHDB = rb_define_module("CHDB");
    rb_define_singleton_method(mCHDB, "query_stable", rb_chdb_query_stable, -1);
    rb_define_singleton_method(mCHDB, "query_stable_v2", rb_chdb_query_stable_v2, -1);
}