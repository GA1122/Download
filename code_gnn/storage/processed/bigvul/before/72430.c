static int pdo_stmt_do_next_rowset(pdo_stmt_t *stmt TSRMLS_DC)
{
	 
	if (stmt->columns) {
		int i;
		struct pdo_column_data *cols = stmt->columns;

		for (i = 0; i < stmt->column_count; i++) {
			efree(cols[i].name);
		}
		efree(stmt->columns);
		stmt->columns = NULL;
		stmt->column_count = 0;
	}

	if (!stmt->methods->next_rowset(stmt TSRMLS_CC)) {
		 
		stmt->executed = 0;
		return 0;
	}

	pdo_stmt_describe_columns(stmt TSRMLS_CC);

	return 1;
}
