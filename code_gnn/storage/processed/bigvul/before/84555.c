create_option_search_table()
{
    int i, j, k;
    int diff1, diff2;
    char *p, *q;

     
    RC_table_size = 0;
    for (j = 0; sections[j].name != NULL; j++) {
	i = 0;
	while (sections[j].params[i].name) {
	    i++;
	    RC_table_size++;
	}
    }

    RC_search_table = New_N(struct rc_search_table, RC_table_size);
    k = 0;
    for (j = 0; sections[j].name != NULL; j++) {
	i = 0;
	while (sections[j].params[i].name) {
	    RC_search_table[k].param = &sections[j].params[i];
	    k++;
	    i++;
	}
    }

    qsort(RC_search_table, RC_table_size, sizeof(struct rc_search_table),
	  (int (*)(const void *, const void *))compare_table);

    diff2 = 0;
    for (i = 0; i < RC_table_size - 1; i++) {
	p = RC_search_table[i].param->name;
	q = RC_search_table[i + 1].param->name;
	for (j = 0; p[j] != '\0' && q[j] != '\0' && p[j] == q[j]; j++) ;
	diff1 = j;
	if (diff1 > diff2)
	    RC_search_table[i].uniq_pos = diff1 + 1;
	else
	    RC_search_table[i].uniq_pos = diff2 + 1;
	diff2 = diff1;
    }
}