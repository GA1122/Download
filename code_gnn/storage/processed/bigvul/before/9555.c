static void php_snmp_session_destructor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	php_snmp_session *session = (php_snmp_session *)rsrc->ptr;
	netsnmp_session_free(&session);
}
