static int do_exec(smart_str *querystr, int expect, PGconn *pg_link, ulong opt TSRMLS_DC)
{
	if (opt & PGSQL_DML_ASYNC) {
		if (PQsendQuery(pg_link, querystr->c)) {
			return 0;
		}
	}
	else {
		PGresult *pg_result;

		pg_result = PQexec(pg_link, querystr->c);
		if (PQresultStatus(pg_result) == expect) {
			PQclear(pg_result);
			return 0;
		} else {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "%s", PQresultErrorMessage(pg_result));
			PQclear(pg_result);
		}
	}

	return -1;
}
