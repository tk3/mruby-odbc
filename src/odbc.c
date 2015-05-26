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

static mrb_odbc_env *mrb_odbc_env_alloc(mrb_state *mrb);
static mrb_value mrb_odbc_env_initialize(mrb_state *mrb, mrb_value self);
static void mrb_odbc_env_free(mrb_state *mrb, void *p);

static const mrb_data_type mrb_odbc_env_type = {
  "mrb_odbc_env", mrb_odbc_env_free,
};

static mrb_odbc_env *mrb_odbc_env_alloc(mrb_state *mrb)
{
  return (mrb_odbc_env *)mrb_malloc(mrb, sizeof(mrb_odbc_env));
}

static mrb_value mrb_odbc_env_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_odbc_env *env;

  env = mrb_odbc_env_alloc(mrb);

  SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env->env);

  DATA_PTR(self) = env;
  DATA_TYPE(self) = &mrb_odbc_env_type;

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


void
mrb_mruby_odbc_gem_init(mrb_state* mrb)
{
  struct RClass *module_odbc;
  struct RClass *class_env;

  module_odbc = mrb_define_module(mrb, "ODBC");

  class_env = mrb_define_class_under(mrb, module_odbc, "Env", mrb->object_class);
  MRB_SET_INSTANCE_TT(class_env, MRB_TT_DATA);
  mrb_define_method(mrb, class_env, "initialize", mrb_odbc_env_initialize, MRB_ARGS_NONE());
}

void
mrb_mruby_odbc_gem_final(mrb_state* mrb)
{
  /* finalizer */
}
