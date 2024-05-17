cJSON *cJSON_ParseWithOpts(const char *value,const char **return_parse_end,int require_null_terminated)
{
	const char *end=0,**ep=return_parse_end?return_parse_end:&global_ep;
	cJSON *c=cJSON_New_Item();
	*ep=0;
	if (!c) return 0;        

	end=parse_value(c,skip(value),ep);
	if (!end)	{cJSON_Delete(c);return 0;}	 

	 
	if (require_null_terminated) {end=skip(end);if (*end) {cJSON_Delete(c);*ep=end;return 0;}}
	if (return_parse_end) *return_parse_end=end;
	return c;
}
