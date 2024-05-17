PHP_FUNCTION(pg_get_notify)
{
	zval *pgsql_link;
	int id = -1;
	long result_type = PGSQL_ASSOC;
	PGconn *pgsql;
	PGnotify *pgsql_notify;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "r|l",
								 &pgsql_link, &result_type) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, &pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	if (!(result_type & PGSQL_BOTH)) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid result type");
		RETURN_FALSE;
	}

	PQconsumeInput(pgsql);
	pgsql_notify = PQnotifies(pgsql);
	if (!pgsql_notify) {
		 
		RETURN_FALSE;
	}
	array_init(return_value);
	if (result_type & PGSQL_NUM) {
		add_index_string(return_value, 0, pgsql_notify->relname, 1);
		add_index_long(return_value, 1, pgsql_notify->be_pid);
#if HAVE_PQPROTOCOLVERSION && HAVE_PQPARAMETERSTATUS 
		if (PQprotocolVersion(pgsql) >= 3 && atof(PQparameterStatus(pgsql, "server_version")) >= 9.0) {
#else 
		if (atof(PG_VERSION) >= 9.0) {
#endif 
#if HAVE_PQPARAMETERSTATUS
			add_index_string(return_value, 2, pgsql_notify->extra, 1);
#endif
		}
	}
	if (result_type & PGSQL_ASSOC) {
		add_assoc_string(return_value, "message", pgsql_notify->relname, 1);
		add_assoc_long(return_value, "pid", pgsql_notify->be_pid);
#if HAVE_PQPROTOCOLVERSION && HAVE_PQPARAMETERSTATUS 
		if (PQprotocolVersion(pgsql) >= 3 && atof(PQparameterStatus(pgsql, "server_version")) >= 9.0) {
#else 
		if (atof(PG_VERSION) >= 9.0) {
#endif 
#if HAVE_PQPARAMETERSTATUS
			add_assoc_string(return_value, "payload", pgsql_notify->extra, 1);
#endif
		}
	}
	PQfreemem(pgsql_notify);
}
 

 
PHP_FUNCTION(pg_get_pid)
{
	zval *pgsql_link;
	int id = -1;
	PGconn *pgsql;

	if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "r",
								 &pgsql_link) == FAILURE) {
		RETURN_FALSE;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, &pgsql_link, id, "PostgreSQL link", le_link, le_plink);

	RETURN_LONG(PQbackendPID(pgsql));
}
 

 
PHP_PGSQL_API int php_pgsql_meta_data(PGconn *pg_link, const char *table_name, zval *meta TSRMLS_DC) 
{
	PGresult *pg_result;
	char *src, *tmp_name, *tmp_name2 = NULL;
	char *escaped;
	smart_str querystr = {0};
	size_t new_len;
	int i, num_rows;
	zval *elem;

	if (!*table_name) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "The table name must be specified");
		return FAILURE;
	}

	src = estrdup(table_name);
	tmp_name = php_strtok_r(src, ".", &tmp_name2);
	
	if (!tmp_name2 || !*tmp_name2) {
		 
		tmp_name2 = tmp_name;
		tmp_name = "public";
	}

	smart_str_appends(&querystr, 
			"SELECT a.attname, a.attnum, t.typname, a.attlen, a.attnotnull, a.atthasdef, a.attndims, t.typtype = 'e' "
			"FROM pg_class as c, pg_attribute a, pg_type t, pg_namespace n "
			"WHERE a.attnum > 0 AND a.attrelid = c.oid AND c.relname = '");
	escaped = (char *)safe_emalloc(strlen(tmp_name2), 2, 1);
	new_len = PQescapeStringConn(pg_link, escaped, tmp_name2, strlen(tmp_name2), NULL);
	if (new_len) {
		smart_str_appendl(&querystr, escaped, new_len);
	}
	efree(escaped);

	smart_str_appends(&querystr, "' AND c.relnamespace = n.oid AND n.nspname = '");
	escaped = (char *)safe_emalloc(strlen(tmp_name), 2, 1);
	new_len = PQescapeStringConn(pg_link, escaped, tmp_name, strlen(tmp_name), NULL);
	if (new_len) {
		smart_str_appendl(&querystr, escaped, new_len);
	}
	efree(escaped);

	smart_str_appends(&querystr, "' AND a.atttypid = t.oid ORDER BY a.attnum;");
	smart_str_0(&querystr);
	efree(src);

	pg_result = PQexec(pg_link, querystr.c);
	if (PQresultStatus(pg_result) != PGRES_TUPLES_OK || (num_rows = PQntuples(pg_result)) == 0) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Table '%s' doesn't exists", table_name);
		smart_str_free(&querystr);
		PQclear(pg_result);
		return FAILURE;
	}
	smart_str_free(&querystr);

	for (i = 0; i < num_rows; i++) {
		char *name;
		MAKE_STD_ZVAL(elem);
		array_init(elem);
		add_assoc_long(elem, "num", atoi(PQgetvalue(pg_result,i,1)));
		add_assoc_string(elem, "type", PQgetvalue(pg_result,i,2), 1);
		add_assoc_long(elem, "len", atoi(PQgetvalue(pg_result,i,3)));
		if (!strcmp(PQgetvalue(pg_result,i,4), "t")) {
			add_assoc_bool(elem, "not null", 1);
		}
		else {
			add_assoc_bool(elem, "not null", 0);
		}
		if (!strcmp(PQgetvalue(pg_result,i,5), "t")) {
			add_assoc_bool(elem, "has default", 1);
		}
		else {
			add_assoc_bool(elem, "has default", 0);
		}
		add_assoc_long(elem, "array dims", atoi(PQgetvalue(pg_result,i,6)));
		if (!strcmp(PQgetvalue(pg_result,i,7), "t")) {
			add_assoc_bool(elem, "is enum", 1);
		}
		else {
			add_assoc_bool(elem, "is enum", 0);
		}
		name = PQgetvalue(pg_result,i,0);
		add_assoc_zval(meta, name, elem);
	}
	PQclear(pg_result);
	
	return SUCCESS;
}

 


 
PHP_FUNCTION(pg_meta_data)
{
	zval *pgsql_link;
	char *table_name;
	uint table_name_len;
	PGconn *pgsql;
	int id = -1;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs",
							  &pgsql_link, &table_name, &table_name_len) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE2(pgsql, PGconn *, &pgsql_link, id, "PostgreSQL link", le_link, le_plink);
	
	array_init(return_value);
	if (php_pgsql_meta_data(pgsql, table_name, return_value TSRMLS_CC) == FAILURE) {
		zval_dtor(return_value);  
		RETURN_FALSE;
	}
	else {
		HashPosition pos;
		zval **val;

		for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(return_value), &pos);
			zend_hash_get_current_data_ex(Z_ARRVAL_P(return_value), (void **)&val, &pos) == SUCCESS;
			zend_hash_move_forward_ex(Z_ARRVAL_P(return_value), &pos)) {
			 
			zend_hash_del_key_or_index(Z_ARRVAL_PP(val), "is enum", sizeof("is enum"), 0, HASH_DEL_KEY);
		}
	}
}
 


 
static php_pgsql_data_type php_pgsql_get_data_type(const char *type_name, size_t len)
{
	 

	 
	if (!strcmp(type_name, "bool")|| !strcmp(type_name, "boolean"))
		return PG_BOOL;
	 
	if (!strcmp(type_name, "oid"))
		return PG_OID;
	 
	if (!strcmp(type_name, "int2") || !strcmp(type_name, "smallint"))
		return PG_INT2;
	if (!strcmp(type_name, "int4") || !strcmp(type_name, "integer"))
		return PG_INT4;
	if (!strcmp(type_name, "int8") || !strcmp(type_name, "bigint"))
		return PG_INT8;
	 
	if (!strcmp(type_name, "float4") || !strcmp(type_name, "real"))
		return PG_FLOAT4;
	if (!strcmp(type_name, "float8") || !strcmp(type_name, "double precision"))
		return PG_FLOAT8;
	if (!strcmp(type_name, "numeric"))
		return PG_NUMERIC;
	if (!strcmp(type_name, "money"))
		return PG_MONEY;
	 
	if (!strcmp(type_name, "text"))
		return PG_TEXT;
	if (!strcmp(type_name, "bpchar") || !strcmp(type_name, "character"))
		return PG_CHAR;
	if (!strcmp(type_name, "varchar") || !strcmp(type_name, "character varying"))
		return PG_VARCHAR;
	 
	if (!strcmp(type_name, "abstime"))
		return PG_UNIX_TIME;
	if (!strcmp(type_name, "reltime"))
		return PG_UNIX_TIME_INTERVAL;
	if (!strcmp(type_name, "tinterval"))
		return PG_UNIX_TIME_INTERVAL;
	if (!strcmp(type_name, "date"))
		return PG_DATE;
	if (!strcmp(type_name, "time"))
		return PG_TIME;
	if (!strcmp(type_name, "time with time zone") || !strcmp(type_name, "timetz"))
		return PG_TIME_WITH_TIMEZONE;
	if (!strcmp(type_name, "timestamp without time zone") || !strcmp(type_name, "timestamp"))
		return PG_TIMESTAMP;
	if (!strcmp(type_name, "timestamp with time zone") || !strcmp(type_name, "timestamptz"))
		return PG_TIMESTAMP_WITH_TIMEZONE;
	if (!strcmp(type_name, "interval"))
		return PG_INTERVAL;
	 
	if (!strcmp(type_name, "bytea"))
		return PG_BYTEA;
	 
	if (!strcmp(type_name, "cidr"))
		return PG_CIDR;
	if (!strcmp(type_name, "inet"))
		return PG_INET;
	if (!strcmp(type_name, "macaddr"))
		return PG_MACADDR;
	 
	if (!strcmp(type_name, "bit"))
		return PG_BIT;
	if (!strcmp(type_name, "bit varying"))
		return PG_VARBIT;
	 
	if (!strcmp(type_name, "line"))
		return PG_LINE;
	if (!strcmp(type_name, "lseg"))
		return PG_LSEG;
	if (!strcmp(type_name, "box"))
		return PG_BOX;
	if (!strcmp(type_name, "path"))
		return PG_PATH;
	if (!strcmp(type_name, "point"))
		return PG_POINT;
	if (!strcmp(type_name, "polygon"))
		return PG_POLYGON;
	if (!strcmp(type_name, "circle"))
		return PG_CIRCLE;

	return PG_UNKNOWN;
}
 

 
static int php_pgsql_convert_match(const char *str, size_t str_len, const char *regex , int icase TSRMLS_DC)
{
	regex_t re;
	regmatch_t *subs;
	int regopt = REG_EXTENDED;
	int regerr, ret = SUCCESS;
	int i;

	 
	for (i = 0; i < str_len; i++) {
		if (str[i] == '\n' ||
			str[i] == '\r' ||
			str[i] == '\0' ) {
			return FAILURE;
		}
	}

	if (icase) {
		regopt |= REG_ICASE;
	}

	regerr = regcomp(&re, regex, regopt);
	if (regerr) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Cannot compile regex");
		regfree(&re);
		return FAILURE;
	}
	subs = (regmatch_t *)ecalloc(sizeof(regmatch_t), re.re_nsub+1);

	regerr = regexec(&re, str, re.re_nsub+1, subs, 0);
	if (regerr == REG_NOMATCH) {
#ifdef PHP_DEBUG
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "'%s' does not match with '%s'", str, regex);
#endif
		ret = FAILURE;
	}
	else if (regerr) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Cannot exec regex");
		ret = FAILURE;
	}
	regfree(&re);
	efree(subs);
	return ret;
}

 

 
static int php_pgsql_add_quotes(zval *src, zend_bool should_free TSRMLS_DC) 
{
	smart_str str = {0};

	assert(Z_TYPE_P(src) == IS_STRING);
	assert(should_free == 1 || should_free == 0);

	smart_str_appendc(&str, 'E');
	smart_str_appendc(&str, '\'');
	smart_str_appendl(&str, Z_STRVAL_P(src), Z_STRLEN_P(src));
	smart_str_appendc(&str, '\'');
	smart_str_0(&str);

	if (should_free) {
		efree(Z_STRVAL_P(src));
	}
	Z_STRVAL_P(src) = str.c;
	Z_STRLEN_P(src) = str.len;

	return SUCCESS;
}
 

#define PGSQL_CONV_CHECK_IGNORE() \
				if (!err && Z_TYPE_P(new_val) == IS_STRING && !strcmp(Z_STRVAL_P(new_val), "NULL")) { \
