static void php_snmp_getvalue(struct variable_list *vars, zval *snmpval, int valueretrieval)
{
	zval val;
	char sbuf[512];
	char *buf = &(sbuf[0]);
	char *dbuf = (char *)NULL;
	int buflen = sizeof(sbuf) - 1;
	int val_len = vars->val_len;

	 

	 
	while ((valueretrieval & SNMP_VALUE_PLAIN) == 0) {
		*buf = '\0';
		if (snprint_value(buf, buflen, vars->name, vars->name_length, vars) == -1) {
			if (val_len > 512*1024) {
				php_error_docref(NULL, E_WARNING, "snprint_value() asks for a buffer more than 512k, Net-SNMP bug?");
				break;
			}
			  
			val_len *= 2;
		} else {
			break;
		}

		if (buf == dbuf) {
			dbuf = (char *)erealloc(dbuf, val_len + 1);
		} else {
			dbuf = (char *)emalloc(val_len + 1);
		}

		if (!dbuf) {
			php_error_docref(NULL, E_WARNING, "emalloc() failed: %s, fallback to static buffer", strerror(errno));
			buf = &(sbuf[0]);
			buflen = sizeof(sbuf) - 1;
			break;
		}

		buf = dbuf;
		buflen = val_len;
	}

	if((valueretrieval & SNMP_VALUE_PLAIN) && val_len > buflen){
		if ((dbuf = (char *)emalloc(val_len + 1))) {
			buf = dbuf;
			buflen = val_len;
		} else {
			php_error_docref(NULL, E_WARNING, "emalloc() failed: %s, fallback to static buffer", strerror(errno));
		}
	}

	if (valueretrieval & SNMP_VALUE_PLAIN) {
		*buf = 0;
		switch (vars->type) {
		case ASN_BIT_STR:		 
			ZVAL_STRINGL(&val, (char *)vars->val.bitstring, vars->val_len);
			break;

		case ASN_OCTET_STR:		 
		case ASN_OPAQUE:		 
			ZVAL_STRINGL(&val, (char *)vars->val.string, vars->val_len);
			break;

		case ASN_NULL:			 
			ZVAL_NULL(&val);
			break;

		case ASN_OBJECT_ID:		 
			snprint_objid(buf, buflen, vars->val.objid, vars->val_len / sizeof(oid));
			ZVAL_STRING(&val, buf);
			break;

		case ASN_IPADDRESS:		 
			snprintf(buf, buflen, "%d.%d.%d.%d",
				 (vars->val.string)[0], (vars->val.string)[1],
				 (vars->val.string)[2], (vars->val.string)[3]);
			buf[buflen]=0;
			ZVAL_STRING(&val, buf);
			break;

		case ASN_COUNTER:		 
		case ASN_GAUGE:			 
		 
		case ASN_TIMETICKS:		 
		case ASN_UINTEGER:		 
			snprintf(buf, buflen, "%lu", *vars->val.integer);
			buf[buflen]=0;
			ZVAL_STRING(&val, buf);
			break;

		case ASN_INTEGER:		 
			snprintf(buf, buflen, "%ld", *vars->val.integer);
			buf[buflen]=0;
			ZVAL_STRING(&val, buf);
			break;

#if defined(NETSNMP_WITH_OPAQUE_SPECIAL_TYPES) || defined(OPAQUE_SPECIAL_TYPES)
		case ASN_OPAQUE_FLOAT:		 
			snprintf(buf, buflen, "%f", *vars->val.floatVal);
			ZVAL_STRING(&val, buf);
			break;

		case ASN_OPAQUE_DOUBLE:		 
			snprintf(buf, buflen, "%Lf", *vars->val.doubleVal);
			ZVAL_STRING(&val, buf);
			break;

		case ASN_OPAQUE_I64:		 
			printI64(buf, vars->val.counter64);
			ZVAL_STRING(&val, buf);
			break;

		case ASN_OPAQUE_U64:		 
#endif
		case ASN_COUNTER64:		 
			printU64(buf, vars->val.counter64);
			ZVAL_STRING(&val, buf);
			break;

		default:
			ZVAL_STRING(&val, "Unknown value type");
			php_error_docref(NULL, E_WARNING, "Unknown value type: %u", vars->type);
			break;
		}
	} else   {
		 
		ZVAL_STRING(&val, buf);
	}

	if (valueretrieval & SNMP_VALUE_OBJECT) {
		object_init(snmpval);
		add_property_long(snmpval, "type", vars->type);
		add_property_zval(snmpval, "value", &val);
	} else  {
		ZVAL_COPY(snmpval, &val);
	}
	zval_ptr_dtor(&val);

	if (dbuf){  
		efree(dbuf);
	}
}
