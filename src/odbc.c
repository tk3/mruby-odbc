#include <mruby.h>
#include <stdio.h>

static mrb_value
mrb_odbc_method(mrb_state *mrb, mrb_value self)
{
  return self;
}

void
mrb_mruby_odbc_gem_init(mrb_state* mrb)
{
  struct RClass *odbc_class = mrb_define_module(mrb, "ODBC");
  mrb_define_class_method(mrb, odbc_class, "call", mrb_odbc_method, MRB_ARGS_NONE());
}

void
mrb_mruby_odbc_gem_final(mrb_state* mrb)
{
  /* finalizer */
}
