search_param(char *name)
{
    size_t b, e, i;
    int cmp;
    int len = strlen(name);

    for (b = 0, e = RC_table_size - 1; b <= e;) {
	i = (b + e) / 2;
	cmp = strncmp(name, RC_search_table[i].param->name, len);

	if (!cmp) {
	    if (len >= RC_search_table[i].uniq_pos) {
		return RC_search_table[i].param;
	    }
	    else {
		while ((cmp =
			strcmp(name, RC_search_table[i].param->name)) <= 0)
		    if (!cmp)
			return RC_search_table[i].param;
		    else if (i == 0)
			return NULL;
		    else
			i--;
		 
		return NULL;
	    }
	}
	else if (cmp < 0) {
	    if (i == 0)
		return NULL;
	    e = i - 1;
	}
	else
	    b = i + 1;
    }
    return NULL;
}