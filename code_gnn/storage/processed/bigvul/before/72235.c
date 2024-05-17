authmethod_is_enabled(Authmethod *method)
{
	if (method == NULL)
		return 0;
	 
	if  (method->enabled == NULL || *method->enabled == 0)
		return 0;
	 
	if  (method->batch_flag != NULL && *method->batch_flag != 0)
		return 0;
	return 1;
}
