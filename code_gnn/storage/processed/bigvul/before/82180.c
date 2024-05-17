mrb_init_kernel(mrb_state *mrb)
{
  struct RClass *krn;

  mrb->kernel_module = krn = mrb_define_module(mrb, "Kernel");                                                     
  mrb_define_class_method(mrb, krn, "block_given?",         mrb_f_block_given_p_m,           MRB_ARGS_NONE());     
  mrb_define_class_method(mrb, krn, "global_variables",     mrb_f_global_variables,          MRB_ARGS_NONE());     
  mrb_define_class_method(mrb, krn, "iterator?",            mrb_f_block_given_p_m,           MRB_ARGS_NONE());     
  mrb_define_class_method(mrb, krn, "local_variables",      mrb_local_variables,             MRB_ARGS_NONE());     
;      
  mrb_define_class_method(mrb, krn, "raise",                mrb_f_raise,                     MRB_ARGS_OPT(2));     

  mrb_define_method(mrb, krn, "singleton_class",            mrb_singleton_class,             MRB_ARGS_NONE());

  mrb_define_method(mrb, krn, "===",                        mrb_equal_m,                     MRB_ARGS_REQ(1));     
  mrb_define_method(mrb, krn, "block_given?",               mrb_f_block_given_p_m,           MRB_ARGS_NONE());     
  mrb_define_method(mrb, krn, "class",                      mrb_obj_class_m,                 MRB_ARGS_NONE());     
  mrb_define_method(mrb, krn, "clone",                      mrb_obj_clone,                   MRB_ARGS_NONE());     
  mrb_define_method(mrb, krn, "dup",                        mrb_obj_dup,                     MRB_ARGS_NONE());     
  mrb_define_method(mrb, krn, "eql?",                       mrb_obj_equal_m,                 MRB_ARGS_REQ(1));     
  mrb_define_method(mrb, krn, "equal?",                     mrb_obj_equal_m,                 MRB_ARGS_REQ(1));     
  mrb_define_method(mrb, krn, "extend",                     mrb_obj_extend_m,                MRB_ARGS_ANY());      
  mrb_define_method(mrb, krn, "freeze",                     mrb_obj_freeze,                  MRB_ARGS_NONE());
  mrb_define_method(mrb, krn, "frozen?",                    mrb_obj_frozen,                  MRB_ARGS_NONE());
  mrb_define_method(mrb, krn, "global_variables",           mrb_f_global_variables,          MRB_ARGS_NONE());     
  mrb_define_method(mrb, krn, "hash",                       mrb_obj_hash,                    MRB_ARGS_NONE());     
  mrb_define_method(mrb, krn, "initialize_copy",            mrb_obj_init_copy,               MRB_ARGS_REQ(1));     
  mrb_define_method(mrb, krn, "inspect",                    mrb_obj_inspect,                 MRB_ARGS_NONE());     
  mrb_define_method(mrb, krn, "instance_of?",               obj_is_instance_of,              MRB_ARGS_REQ(1));     
  mrb_define_method(mrb, krn, "instance_variable_defined?", mrb_obj_ivar_defined,            MRB_ARGS_REQ(1));     
  mrb_define_method(mrb, krn, "instance_variable_get",      mrb_obj_ivar_get,                MRB_ARGS_REQ(1));     
  mrb_define_method(mrb, krn, "instance_variable_set",      mrb_obj_ivar_set,                MRB_ARGS_REQ(2));     
  mrb_define_method(mrb, krn, "instance_variables",         mrb_obj_instance_variables,      MRB_ARGS_NONE());     
  mrb_define_method(mrb, krn, "is_a?",                      mrb_obj_is_kind_of_m,            MRB_ARGS_REQ(1));     
  mrb_define_method(mrb, krn, "iterator?",                  mrb_f_block_given_p_m,           MRB_ARGS_NONE());     
  mrb_define_method(mrb, krn, "kind_of?",                   mrb_obj_is_kind_of_m,            MRB_ARGS_REQ(1));     
  mrb_define_method(mrb, krn, "local_variables",            mrb_local_variables,             MRB_ARGS_NONE());     
#ifdef MRB_DEFAULT_METHOD_MISSING
  mrb_define_method(mrb, krn, "method_missing",             mrb_obj_missing,                 MRB_ARGS_ANY());      
#endif
  mrb_define_method(mrb, krn, "methods",                    mrb_obj_methods_m,               MRB_ARGS_OPT(1));     
  mrb_define_method(mrb, krn, "nil?",                       mrb_false,                       MRB_ARGS_NONE());     
  mrb_define_method(mrb, krn, "object_id",                  mrb_obj_id_m,                    MRB_ARGS_NONE());     
  mrb_define_method(mrb, krn, "private_methods",            mrb_obj_private_methods,         MRB_ARGS_OPT(1));     
  mrb_define_method(mrb, krn, "protected_methods",          mrb_obj_protected_methods,       MRB_ARGS_OPT(1));     
  mrb_define_method(mrb, krn, "public_methods",             mrb_obj_public_methods,          MRB_ARGS_OPT(1));     
  mrb_define_method(mrb, krn, "raise",                      mrb_f_raise,                     MRB_ARGS_ANY());      
  mrb_define_method(mrb, krn, "remove_instance_variable",   mrb_obj_remove_instance_variable,MRB_ARGS_REQ(1));     
  mrb_define_method(mrb, krn, "respond_to?",                obj_respond_to,                  MRB_ARGS_ANY());      
  mrb_define_method(mrb, krn, "send",                       mrb_f_send,                      MRB_ARGS_ANY());      
  mrb_define_method(mrb, krn, "singleton_methods",          mrb_obj_singleton_methods_m,     MRB_ARGS_OPT(1));     
  mrb_define_method(mrb, krn, "define_singleton_method",    mod_define_singleton_method,     MRB_ARGS_ANY());
  mrb_define_method(mrb, krn, "to_s",                       mrb_any_to_s,                    MRB_ARGS_NONE());     
  mrb_define_method(mrb, krn, "__case_eqq",                 mrb_obj_ceqq,                    MRB_ARGS_REQ(1));     

  mrb_define_method(mrb, krn, "class_defined?",             mrb_krn_class_defined,           MRB_ARGS_REQ(1));

  mrb_include_module(mrb, mrb->object_class, mrb->kernel_module);
  mrb_alias_method(mrb, mrb->module_class, mrb_intern_lit(mrb, "dup"), mrb_intern_lit(mrb, "clone"));
}
