cJSON *cJSON_CreateString(const char *string)	{cJSON *item=cJSON_New_Item();if(item){item->type=cJSON_String;item->valuestring=cJSON_strdup(string);if(!item->valuestring){cJSON_Delete(item);return 0;}}return item;}