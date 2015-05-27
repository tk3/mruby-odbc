#include <stdio.h>
#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "sql.h"
#include "sqlext.h"

typedef struct {
  SQLHENV env;
} mrb_odbc_env;

typedef struct {
  SQLHDBC conn;
} mrb_odbc_conn;

static mrb_odbc_env *mrb_odbc_env_alloc(mrb_state *mrb);
static mrb_value mrb_odbc_env_initialize(mrb_state *mrb, mrb_value self);
static mrb_value mrb_odbc_env_set_attr(mrb_state *mrb, mrb_value self);
static void mrb_odbc_env_free(mrb_state *mrb, void *p);

static mrb_odbc_conn *mrb_odbc_conn_alloc(mrb_state *mrb);
static mrb_value mrb_odbc_conn_initialize(mrb_state *mrb, mrb_value self);
static void mrb_odbc_conn_free(mrb_state *mrb, void *p);

static const mrb_data_type mrb_odbc_env_type = {
  "mrb_odbc_env", mrb_odbc_env_free,
};
static const mrb_data_type mrb_odbc_conn_type = {
  "mrb_odbc_conn", mrb_odbc_conn_free,
};

static mrb_odbc_env *mrb_odbc_env_alloc(mrb_state *mrb)
{
  return (mrb_odbc_env *)mrb_malloc(mrb, sizeof(mrb_odbc_env));
}

static mrb_value mrb_odbc_env_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_odbc_env *env;
  SQLRETURN r;

  env = mrb_odbc_env_alloc(mrb);

  r = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env->env);
  if (!(r == SQL_SUCCESS || r == SQL_SUCCESS_WITH_INFO)) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "Failed to create Env Handle.");
  }

  DATA_PTR(self) = env;
  DATA_TYPE(self) = &mrb_odbc_env_type;

  return self;
}

static mrb_value mrb_odbc_env_set_attr(mrb_state *mrb, mrb_value self)
{
  mrb_int attr;
  mrb_int val;
  mrb_odbc_env *env;

  mrb_get_args(mrb, "ii", &attr, &val);

  env = mrb_get_datatype(mrb, self, &mrb_odbc_env_type);

  SQLSetEnvAttr(env->env, attr, (SQLPOINTER *)val, 0);

  return self;
}

static void mrb_odbc_env_free(mrb_state *mrb, void *p)
{
  mrb_odbc_env *env = (mrb_odbc_env *)p;
  if (env->env != NULL) {
    SQLFreeHandle(SQL_HANDLE_ENV, env->env);
  }
  mrb_free(mrb, env);
}


static mrb_odbc_conn *mrb_odbc_conn_alloc(mrb_state *mrb)
{
  return (mrb_odbc_conn *)mrb_malloc(mrb, sizeof(mrb_odbc_conn));
}

static mrb_value mrb_odbc_conn_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_odbc_conn *conn;
  mrb_value arg_env;
  mrb_odbc_env *env;
  SQLRETURN r;

  mrb_get_args(mrb, "o", &arg_env);

  env = mrb_get_datatype(mrb, arg_env, &mrb_odbc_env_type);

  conn = mrb_odbc_conn_alloc(mrb);

  r = SQLAllocHandle(SQL_HANDLE_DBC, env->env, &(conn->conn));
  if (!(r == SQL_SUCCESS || r == SQL_SUCCESS_WITH_INFO)) {
    mrb_raise(mrb, E_RUNTIME_ERROR, "Failed to create Conn Handle.");
  }

  DATA_PTR(self) = conn;
  DATA_TYPE(self) = &mrb_odbc_conn_type;

  return self;
}

static void mrb_odbc_conn_free(mrb_state *mrb, void *p)
{
  mrb_odbc_conn *conn = (mrb_odbc_conn *)p;
  if (conn->conn != NULL) {
    SQLFreeHandle(SQL_HANDLE_DBC, conn->conn);
  }
  mrb_free(mrb, conn);
}


void
mrb_mruby_odbc_gem_init(mrb_state* mrb)
{
  struct RClass *module_odbc;
  struct RClass *class_env;
  struct RClass *class_conn;

  module_odbc = mrb_define_module(mrb, "ODBC");

  class_env = mrb_define_class_under(mrb, module_odbc, "Env", mrb->object_class);
  MRB_SET_INSTANCE_TT(class_env, MRB_TT_DATA);
  mrb_define_method(mrb, class_env, "initialize", mrb_odbc_env_initialize, MRB_ARGS_NONE());
  mrb_define_method(mrb, class_env, "set_attr", mrb_odbc_env_set_attr, MRB_ARGS_REQ(2));

  mrb_define_const(mrb, class_env, "ODBC_VERSION", mrb_fixnum_value(SQL_ATTR_ODBC_VERSION));
  mrb_define_const(mrb, class_env, "VERSION_2", mrb_fixnum_value(SQL_OV_ODBC2));
  mrb_define_const(mrb, class_env, "VERSION_3", mrb_fixnum_value(SQL_OV_ODBC3));

  mrb_define_const(mrb, class_env, "CONNECTION_POOLING", mrb_fixnum_value(SQL_ATTR_CONNECTION_POOLING));
  mrb_define_const(mrb, class_env, "CP_OFF", mrb_fixnum_value(SQL_CP_OFF));
  mrb_define_const(mrb, class_env, "CP_ONE_PER_DRIVER", mrb_fixnum_value(SQL_CP_ONE_PER_DRIVER));
  mrb_define_const(mrb, class_env, "CP_ONE_PER_ENV", mrb_fixnum_value(SQL_CP_ONE_PER_HENV));
  mrb_define_const(mrb, class_env, "CP_DEFAULT", mrb_fixnum_value(SQL_CP_DEFAULT));

  mrb_define_const(mrb, class_env, "CP_MATCH", mrb_fixnum_value(SQL_ATTR_CP_MATCH));
  mrb_define_const(mrb, class_env, "CP_STRICT_MATCH", mrb_fixnum_value(SQL_CP_STRICT_MATCH));
  mrb_define_const(mrb, class_env, "CP_RELAXED_MATCH", mrb_fixnum_value(SQL_CP_RELAXED_MATCH));
  mrb_define_const(mrb, class_env, "CP_MATCH_DEFAULT", mrb_fixnum_value(SQL_CP_MATCH_DEFAULT));


  class_conn = mrb_define_class_under(mrb, module_odbc, "Conn", mrb->object_class);
  MRB_SET_INSTANCE_TT(class_conn, MRB_TT_DATA);
  mrb_define_method(mrb, class_conn, "initialize", mrb_odbc_conn_initialize, MRB_ARGS_REQ(1));
}

void
mrb_mruby_odbc_gem_final(mrb_state* mrb)
{
  /* finalizer */
}
