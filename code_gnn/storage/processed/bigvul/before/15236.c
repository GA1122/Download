static int php_pgsql_flush_query(PGconn *pgsql TSRMLS_DC) 
{
	PGresult *res;
	int leftover = 0;
	
	if (PQ_SETNONBLOCKING(pgsql, 1)) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE,"Cannot set connection to nonblocking mode");
		return -1;
	}
	while ((res = PQgetResult(pgsql))) {
		PQclear(res);
		leftover++;
	}
	PQ_SETNONBLOCKING(pgsql, 0);
	return leftover;
}
