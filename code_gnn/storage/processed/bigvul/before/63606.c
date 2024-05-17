SAPI_API SAPI_INPUT_FILTER_FUNC(php_default_input_filter)
{
	 
	if(new_val_len) *new_val_len = val_len;
	return 1;
}
