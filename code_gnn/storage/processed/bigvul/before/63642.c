int get_max_column_count(GSList *items, COLUMN_LEN_FUNC len_func,
			 int max_width, int max_columns,
			 int item_extra, int item_min_size,
			 int **save_column_widths, int *rows)
{
        GSList *tmp;
	int **columns, *columns_width, *columns_rows;
	int item_pos, items_count;
	int ret, len, max_len, n, col;

	items_count = g_slist_length(items);
	if (items_count == 0) {
		*save_column_widths = NULL;
                *rows = 0;
		return 0;
	}

	len = max_width/(item_extra+item_min_size);
        if (len <= 0) len = 1;
	if (max_columns <= 0 || len < max_columns)
                max_columns = len;

	columns = g_new0(int *, max_columns);
	columns_width = g_new0(int, max_columns);
	columns_rows = g_new0(int, max_columns);

	for (n = 1; n < max_columns; n++) {
		columns[n] = g_new0(int, n+1);
		columns_rows[n] = items_count <= n+1 ? 1 :
                        (items_count+n)/(n+1);
	}

	 
        item_pos = 0; max_len = 0;
	for (tmp = items; tmp != NULL; tmp = tmp->next) {
		len = item_extra+len_func(tmp->data);
		if (max_len < len)
			max_len = len;

		for (n = 1; n < max_columns; n++) {
			if (columns_width[n] > max_width)
				continue;  

			col = item_pos/columns_rows[n];
			if (columns[n][col] < len) {
				columns_width[n] += len-columns[n][col];
                                columns[n][col] = len;
			}
		}

                item_pos++;
	}

	for (n = max_columns-1; n >= 1; n--) {
		if (columns_width[n] <= max_width &&
		    columns[n][n] > 0)
                        break;
	}
        ret = n+1;

	*save_column_widths = g_new(int, ret);
	if (ret == 1) {
                **save_column_widths = max_len;
                *rows = 1;
	} else {
		memcpy(*save_column_widths, columns[ret-1], sizeof(int)*ret);
		*rows = columns_rows[ret-1];
	}

	for (n = 1; n < max_columns; n++)
                g_free(columns[n]);
	g_free(columns_width);
	g_free(columns_rows);
	g_free(columns);

        return ret;
}