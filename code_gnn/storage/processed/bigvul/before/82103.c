mrb_mod_append_features(mrb_state *mrb, mrb_value mod)
{
  mrb_value klass;

  mrb_check_type(mrb, mod, MRB_TT_MODULE);
  mrb_get_args(mrb, "C", &klass);
  mrb_include_module(mrb, mrb_class_ptr(klass), mrb_class_ptr(mod));
  return mod;
}