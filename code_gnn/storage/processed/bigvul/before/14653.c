PHP_FUNCTION(localeconv)
{
	zval grouping, mon_grouping;
	int len, i;

	 
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	array_init(return_value);
	array_init(&grouping);
	array_init(&mon_grouping);

#ifdef HAVE_LOCALECONV
	{
		struct lconv currlocdata;

		localeconv_r( &currlocdata );

		 
		len = (int)strlen(currlocdata.grouping);

		for (i = 0; i < len; i++) {
			add_index_long(&grouping, i, currlocdata.grouping[i]);
		}

		 
		len = (int)strlen(currlocdata.mon_grouping);

		for (i = 0; i < len; i++) {
			add_index_long(&mon_grouping, i, currlocdata.mon_grouping[i]);
		}

		add_assoc_string(return_value, "decimal_point",     currlocdata.decimal_point);
		add_assoc_string(return_value, "thousands_sep",     currlocdata.thousands_sep);
		add_assoc_string(return_value, "int_curr_symbol",   currlocdata.int_curr_symbol);
		add_assoc_string(return_value, "currency_symbol",   currlocdata.currency_symbol);
		add_assoc_string(return_value, "mon_decimal_point", currlocdata.mon_decimal_point);
		add_assoc_string(return_value, "mon_thousands_sep", currlocdata.mon_thousands_sep);
		add_assoc_string(return_value, "positive_sign",     currlocdata.positive_sign);
		add_assoc_string(return_value, "negative_sign",     currlocdata.negative_sign);
		add_assoc_long(  return_value, "int_frac_digits",   currlocdata.int_frac_digits);
		add_assoc_long(  return_value, "frac_digits",       currlocdata.frac_digits);
		add_assoc_long(  return_value, "p_cs_precedes",     currlocdata.p_cs_precedes);
		add_assoc_long(  return_value, "p_sep_by_space",    currlocdata.p_sep_by_space);
		add_assoc_long(  return_value, "n_cs_precedes",     currlocdata.n_cs_precedes);
		add_assoc_long(  return_value, "n_sep_by_space",    currlocdata.n_sep_by_space);
		add_assoc_long(  return_value, "p_sign_posn",       currlocdata.p_sign_posn);
		add_assoc_long(  return_value, "n_sign_posn",       currlocdata.n_sign_posn);
	}
#else
	 

	add_index_long(&grouping, 0, -1);
	add_index_long(&mon_grouping, 0, -1);

	add_assoc_string(return_value, "decimal_point",     "\x2E");
	add_assoc_string(return_value, "thousands_sep",     "");
	add_assoc_string(return_value, "int_curr_symbol",   "");
	add_assoc_string(return_value, "currency_symbol",   "");
	add_assoc_string(return_value, "mon_decimal_point", "\x2E");
	add_assoc_string(return_value, "mon_thousands_sep", "");
	add_assoc_string(return_value, "positive_sign",     "");
	add_assoc_string(return_value, "negative_sign",     "");
	add_assoc_long(  return_value, "int_frac_digits",   CHAR_MAX);
	add_assoc_long(  return_value, "frac_digits",       CHAR_MAX);
	add_assoc_long(  return_value, "p_cs_precedes",     CHAR_MAX);
	add_assoc_long(  return_value, "p_sep_by_space",    CHAR_MAX);
	add_assoc_long(  return_value, "n_cs_precedes",     CHAR_MAX);
	add_assoc_long(  return_value, "n_sep_by_space",    CHAR_MAX);
	add_assoc_long(  return_value, "p_sign_posn",       CHAR_MAX);
	add_assoc_long(  return_value, "n_sign_posn",       CHAR_MAX);
#endif

	zend_hash_str_update(Z_ARRVAL_P(return_value), "grouping", sizeof("grouping")-1, &grouping);
	zend_hash_str_update(Z_ARRVAL_P(return_value), "mon_grouping", sizeof("mon_grouping")-1, &mon_grouping);
}
