static inline zend_long object_common1(UNSERIALIZE_PARAMETER, zend_class_entry *ce)
{
	zend_long elements;

	if( *p >= max - 2) {
		zend_error(E_WARNING, "Bad unserialize data");
		return -1;
	}

	elements = parse_iv2((*p) + 2, p);

	(*p) += 2;

	if (ce->serialize == NULL) {
		object_init_ex(rval, ce);
	} else {
		 
		zend_error(E_WARNING, "Erroneous data format for unserializing '%s'", ZSTR_VAL(ce->name));
		return -1;
	}

	return elements;
}
