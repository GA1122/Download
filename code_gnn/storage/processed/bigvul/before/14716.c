PHP_FUNCTION(pg_ping)
{
	zval *pgsql_link;
	int id;
	PGconn *pgsql;
	PGresult *res;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "r", &pgsql_link) == SUCCESS) {
		id = -1;
	} else {
		pgsql_link = NULL;
		id = PGG(default_link);
	}
	if (pgsql_link == NULL && id == -1) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, &pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	 
	res = PQexec(pgsql, "SELECT 1;");
	PQclear(res);

	 
	if (PQstatus(pgsql) == CONNECTION_OK)
		RETURN_TRUE;

	 
	PQreset(pgsql);
	if (PQstatus(pgsql) == CONNECTION_OK) {
		RETURN_TRUE;
	}
	RETURN_FALSE;
}
