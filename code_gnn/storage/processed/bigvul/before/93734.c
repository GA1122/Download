static const char *parse_object(cJSON *item,const char *value,const char **ep)
{
	cJSON *child;
	if (*value!='{')	{*ep=value;return 0;}	 
	
	item->type=cJSON_Object;
	value=skip(value+1);
	if (*value=='}') return value+1;	 
	
	item->child=child=cJSON_New_Item();
	if (!item->child) return 0;
	value=skip(parse_string(child,skip(value),ep));
	if (!value) return 0;
	child->string=child->valuestring;child->valuestring=0;
	if (*value!=':') {*ep=value;return 0;}	 
	value=skip(parse_value(child,skip(value+1),ep));	 
	if (!value) return 0;
	
	while (*value==',')
	{
		cJSON *new_item;
		if (!(new_item=cJSON_New_Item()))	return 0;  
		child->next=new_item;new_item->prev=child;child=new_item;
		value=skip(parse_string(child,skip(value+1),ep));
		if (!value) return 0;
		child->string=child->valuestring;child->valuestring=0;
		if (*value!=':') {*ep=value;return 0;}	 
		value=skip(parse_value(child,skip(value+1),ep));	 
		if (!value) return 0;
	}
	
	if (*value=='}') return value+1;	 
