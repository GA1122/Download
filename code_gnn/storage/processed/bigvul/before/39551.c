void license_free_scope_list(SCOPE_LIST* scopeList)
{
	UINT32 i;

	 
	for (i = 0; i < scopeList->count; i++)
	{
		free(scopeList->array[i].data);
	}

	free(scopeList->array);
	free(scopeList);
}
