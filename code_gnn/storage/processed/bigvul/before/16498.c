param_functions* get_param_functions()
{
	config_p_funcs.set_param_func(&param);
	config_p_funcs.set_param_bool_int_func(&param_boolean_int);
	config_p_funcs.set_param_wo_default_func(&param_without_default);

	return &config_p_funcs;
}
