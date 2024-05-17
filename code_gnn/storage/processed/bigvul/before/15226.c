static char *get_field_name(PGconn *pgsql, Oid oid, HashTable *list TSRMLS_DC)
{
	PGresult *result;
	smart_str str = {0};
	zend_rsrc_list_entry *field_type;
	char *ret=NULL;

	 
	smart_str_appends(&str, "pgsql_oid_");
	smart_str_append_unsigned(&str, oid);
	smart_str_0(&str);

	if (zend_hash_find(list,str.c,str.len+1,(void **) &field_type)==SUCCESS) {
		ret = estrdup((char *)field_type->ptr);
	} else {  
		int i,num_rows;
		int oid_offset,name_offset;
		char *tmp_oid, *end_ptr, *tmp_name;
		zend_rsrc_list_entry new_oid_entry;

		if ((result = PQexec(pgsql,"select oid,typname from pg_type")) == NULL || PQresultStatus(result) != PGRES_TUPLES_OK) {
			if (result) {
				PQclear(result);
			}
			smart_str_free(&str);
			return STR_EMPTY_ALLOC();
		}
		num_rows = PQntuples(result);
		oid_offset = PQfnumber(result,"oid");
		name_offset = PQfnumber(result,"typname");

		for (i=0; i<num_rows; i++) {
			if ((tmp_oid = PQgetvalue(result,i,oid_offset))==NULL) {
				continue;
			}
			
			str.len = 0;
			smart_str_appends(&str, "pgsql_oid_");
			smart_str_appends(&str, tmp_oid);
			smart_str_0(&str);

			if ((tmp_name = PQgetvalue(result,i,name_offset))==NULL) {
				continue;
			}
			Z_TYPE(new_oid_entry) = le_string;
			new_oid_entry.ptr = estrdup(tmp_name);
			zend_hash_update(list,str.c,str.len+1,(void *) &new_oid_entry, sizeof(zend_rsrc_list_entry), NULL);
			if (!ret && strtoul(tmp_oid, &end_ptr, 10)==oid) {
				ret = estrdup(tmp_name);
			}
		}
		PQclear(result);
	}

	smart_str_free(&str);
	return ret;
}
