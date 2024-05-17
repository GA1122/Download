 static int php_var_unserialize_internal(UNSERIALIZE_PARAMETER)
 {
 	const unsigned char *cursor, *limit, *marker, *start;
 	zval *rval_ref;
 
 	limit = max;
 	cursor = *p;
 
 	if (YYCURSOR >= YYLIMIT) {
 		return 0;
 	}
 
 	if (var_hash && (*p)[0] != 'R') {
 		var_push(var_hash, rval);
 	}
 
 	start = cursor;
 
 
 #line 554 "ext/standard/var_unserializer.c"
 {
 	YYCTYPE yych;
 	static const unsigned char yybm[] = {
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		128, 128, 128, 128, 128, 128, 128, 128, 
 		128, 128,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 		  0,   0,   0,   0,   0,   0,   0,   0, 
 	};
 
 	if ((YYLIMIT - YYCURSOR) < 7) YYFILL(7);
 	yych = *YYCURSOR;
 	switch (yych) {
 	case 'C':
 	case 'O':	goto yy13;
 	case 'N':	goto yy5;
 	case 'R':	goto yy2;
 	case 'S':	goto yy10;
 	case 'a':	goto yy11;
 	case 'b':	goto yy6;
 	case 'd':	goto yy8;
 	case 'i':	goto yy7;
 	case 'o':	goto yy12;
 	case 'r':	goto yy4;
 	case 's':	goto yy9;
 	case '}':	goto yy14;
 	default:	goto yy16;
 	}
 yy2:
 	yych = *(YYMARKER = ++YYCURSOR);
 	if (yych == ':') goto yy95;
 yy3:
 #line 884 "ext/standard/var_unserializer.re"
 	{ return 0; }
 #line 580 "ext/standard/var_unserializer.c"
 yy4:
 	yych = *(YYMARKER = ++YYCURSOR);
 	if (yych == ':') goto yy89;
 	goto yy3;
 yy5:
 	yych = *++YYCURSOR;
 	if (yych == ';') goto yy87;
 	goto yy3;
 yy6:
 	yych = *(YYMARKER = ++YYCURSOR);
 	if (yych == ':') goto yy83;
 	goto yy3;
 yy7:
 	yych = *(YYMARKER = ++YYCURSOR);
 	if (yych == ':') goto yy77;
 	goto yy3;
 yy8:
 	yych = *(YYMARKER = ++YYCURSOR);
 	if (yych == ':') goto yy53;
 	goto yy3;
 yy9:
 	yych = *(YYMARKER = ++YYCURSOR);
 	if (yych == ':') goto yy46;
 	goto yy3;
 yy10:
 	yych = *(YYMARKER = ++YYCURSOR);
 	if (yych == ':') goto yy39;
 	goto yy3;
 yy11:
 	yych = *(YYMARKER = ++YYCURSOR);
 	if (yych == ':') goto yy32;
 	goto yy3;
 yy12:
 	yych = *(YYMARKER = ++YYCURSOR);
 	if (yych == ':') goto yy25;
 	goto yy3;
 yy13:
 	yych = *(YYMARKER = ++YYCURSOR);
 	if (yych == ':') goto yy17;
 	goto yy3;
 yy14:
 	++YYCURSOR;
 #line 878 "ext/standard/var_unserializer.re"
 	{
 	 
 	php_error_docref(NULL, E_NOTICE, "Unexpected end of serialized data");
 	return 0;  
 }
 #line 629 "ext/standard/var_unserializer.c"
 yy16:
 	yych = *++YYCURSOR;
 	goto yy3;
 yy17:
 	yych = *++YYCURSOR;
 	if (yybm[0+yych] & 128) {
 		goto yy20;
 	}
 	if (yych == '+') goto yy19;
 yy18:
 	YYCURSOR = YYMARKER;
 	goto yy3;
 yy19:
 	yych = *++YYCURSOR;
 	if (yybm[0+yych] & 128) {
 		goto yy20;
 	}
 	goto yy18;
 yy20:
 	++YYCURSOR;
 	if ((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
 	yych = *YYCURSOR;
  	if (yybm[0+yych] & 128) {
  		goto yy20;
  	}
	if (yych != ':') goto yy18;
// 	if (yych <= '/') goto yy18;
// 	if (yych >= ';') goto yy18;
  	yych = *++YYCURSOR;
  	if (yych != '"') goto yy18;
  	++YYCURSOR;
 #line 733 "ext/standard/var_unserializer.re"
 	{
 	size_t len, len2, len3, maxlen;
 	zend_long elements;
 	char *str;
 	zend_string *class_name;
 	zend_class_entry *ce;
 	int incomplete_class = 0;
 
 	int custom_object = 0;
 
 	zval user_func;
 	zval retval;
 	zval args[1];
 
     if (!var_hash) return 0;
 	if (*start == 'C') {
 		custom_object = 1;
 	}
 
 	len2 = len = parse_uiv(start + 2);
 	maxlen = max - YYCURSOR;
 	if (maxlen < len || len == 0) {
 		*p = start + 2;
 		return 0;
 	}
 
 	str = (char*)YYCURSOR;
 
 	YYCURSOR += len;
 
 	if (*(YYCURSOR) != '"') {
 		*p = YYCURSOR;
 		return 0;
 	}
 	if (*(YYCURSOR+1) != ':') {
 		*p = YYCURSOR+1;
 		return 0;
 	}
 
 	len3 = strspn(str, "0123456789_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\177\200\201\202\203\204\205\206\207\210\211\212\213\214\215\216\217\220\221\222\223\224\225\226\227\230\231\232\233\234\235\236\237\240\241\242\243\244\245\246\247\250\251\252\253\254\255\256\257\260\261\262\263\264\265\266\267\270\271\272\273\274\275\276\277\300\301\302\303\304\305\306\307\310\311\312\313\314\315\316\317\320\321\322\323\324\325\326\327\330\331\332\333\334\335\336\337\340\341\342\343\344\345\346\347\350\351\352\353\354\355\356\357\360\361\362\363\364\365\366\367\370\371\372\373\374\375\376\377\\");
 	if (len3 != len)
 	{
 		*p = YYCURSOR + len3 - len;
 		return 0;
 	}
 
 	class_name = zend_string_init(str, len, 0);
 
 	do {
 		if(!unserialize_allowed_class(class_name, classes)) {
 			incomplete_class = 1;
 			ce = PHP_IC_ENTRY;
 			break;
 		}
 
 		 
 		BG(serialize_lock)++;
 		ce = zend_lookup_class(class_name);
 		if (ce) {
 			BG(serialize_lock)--;
 			if (EG(exception)) {
 				zend_string_release(class_name);
 				return 0;
 			}
 			break;
 		}
 		BG(serialize_lock)--;
 
 		if (EG(exception)) {
 			zend_string_release(class_name);
 			return 0;
 		}
 
 		 
 		if ((PG(unserialize_callback_func) == NULL) || (PG(unserialize_callback_func)[0] == '\0')) {
 			incomplete_class = 1;
 			ce = PHP_IC_ENTRY;
 			break;
 		}
 
 		 
 		ZVAL_STRING(&user_func, PG(unserialize_callback_func));
 
 		ZVAL_STR_COPY(&args[0], class_name);
 		BG(serialize_lock)++;
 		if (call_user_function_ex(CG(function_table), NULL, &user_func, &retval, 1, args, 0, NULL) != SUCCESS) {
 			BG(serialize_lock)--;
 			if (EG(exception)) {
 				zend_string_release(class_name);
 				zval_ptr_dtor(&user_func);
 				zval_ptr_dtor(&args[0]);
 				return 0;
 			}
 			php_error_docref(NULL, E_WARNING, "defined (%s) but not found", Z_STRVAL(user_func));
 			incomplete_class = 1;
 			ce = PHP_IC_ENTRY;
 			zval_ptr_dtor(&user_func);
 			zval_ptr_dtor(&args[0]);
 			break;
 		}
 		BG(serialize_lock)--;
 		zval_ptr_dtor(&retval);
 		if (EG(exception)) {
 			zend_string_release(class_name);
 			zval_ptr_dtor(&user_func);
 			zval_ptr_dtor(&args[0]);
 			return 0;
 		}
 
 		 
 		if ((ce = zend_lookup_class(class_name)) == NULL) {
 			php_error_docref(NULL, E_WARNING, "Function %s() hasn't defined the class it was called for", Z_STRVAL(user_func));
 			incomplete_class = 1;
 			ce = PHP_IC_ENTRY;
 		}
 
 		zval_ptr_dtor(&user_func);
 		zval_ptr_dtor(&args[0]);
 		break;
 	} while (1);
 
 	*p = YYCURSOR;
 
 	if (custom_object) {
 		int ret;
 
 		ret = object_custom(UNSERIALIZE_PASSTHRU, ce);
 
 		if (ret && incomplete_class) {
 			php_store_class_name(rval, ZSTR_VAL(class_name), len2);
 		}
 		zend_string_release(class_name);
 		return ret;
 	}
 
 	elements = object_common1(UNSERIALIZE_PASSTHRU, ce);
 
 	if (incomplete_class) {
 		php_store_class_name(rval, ZSTR_VAL(class_name), len2);
 	}
 	zend_string_release(class_name);
  
  	return object_common2(UNSERIALIZE_PASSTHRU, elements);
  }
#line 804 "ext/standard/var_unserializer.c"
// #line 805 "ext/standard/var_unserializer.c"
  yy25:
  	yych = *++YYCURSOR;
  	if (yych <= ',') {
 		if (yych != '+') goto yy18;
 	} else {
 		if (yych <= '-') goto yy26;
 		if (yych <= '/') goto yy18;
 		if (yych <= '9') goto yy27;
 		goto yy18;
 	}
 yy26:
 	yych = *++YYCURSOR;
 	if (yych <= '/') goto yy18;
 	if (yych >= ':') goto yy18;
 yy27:
 	++YYCURSOR;
 	if ((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
 	yych = *YYCURSOR;
 	if (yych <= '/') goto yy18;
 	if (yych <= '9') goto yy27;
 	if (yych >= ';') goto yy18;
 	yych = *++YYCURSOR;
 	if (yych != '"') goto yy18;
 	++YYCURSOR;
 #line 726 "ext/standard/var_unserializer.re"
 	{
     if (!var_hash) return 0;
 
  	return object_common2(UNSERIALIZE_PASSTHRU,
  			object_common1(UNSERIALIZE_PASSTHRU, ZEND_STANDARD_CLASS_DEF_PTR));
  }
#line 836 "ext/standard/var_unserializer.c"
// #line 837 "ext/standard/var_unserializer.c"
  yy32:
  	yych = *++YYCURSOR;
  	if (yych == '+') goto yy33;
 	if (yych <= '/') goto yy18;
 	if (yych <= '9') goto yy34;
 	goto yy18;
 yy33:
 	yych = *++YYCURSOR;
 	if (yych <= '/') goto yy18;
 	if (yych >= ':') goto yy18;
 yy34:
 	++YYCURSOR;
 	if ((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
 	yych = *YYCURSOR;
 	if (yych <= '/') goto yy18;
 	if (yych <= '9') goto yy34;
 	if (yych >= ';') goto yy18;
 	yych = *++YYCURSOR;
 	if (yych != '{') goto yy18;
 	++YYCURSOR;
 #line 702 "ext/standard/var_unserializer.re"
 	{
 	zend_long elements = parse_iv(start + 2);
 	 
 	*p = YYCURSOR;
     if (!var_hash) return 0;
 
 	if (elements < 0) {
 		return 0;
 	}
 
 	array_init_size(rval, elements);
 	if (elements) {
 		 
 		zend_hash_real_init(Z_ARRVAL_P(rval), 0);
 	}
 
 	if (!process_nested_data(UNSERIALIZE_PASSTHRU, Z_ARRVAL_P(rval), elements, 0)) {
 		return 0;
 	}
  
  	return finish_nested_data(UNSERIALIZE_PASSTHRU);
  }
#line 881 "ext/standard/var_unserializer.c"
// #line 882 "ext/standard/var_unserializer.c"
  yy39:
  	yych = *++YYCURSOR;
  	if (yych == '+') goto yy40;
 	if (yych <= '/') goto yy18;
 	if (yych <= '9') goto yy41;
 	goto yy18;
 yy40:
 	yych = *++YYCURSOR;
 	if (yych <= '/') goto yy18;
 	if (yych >= ':') goto yy18;
 yy41:
 	++YYCURSOR;
 	if ((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
 	yych = *YYCURSOR;
 	if (yych <= '/') goto yy18;
 	if (yych <= '9') goto yy41;
 	if (yych >= ';') goto yy18;
 	yych = *++YYCURSOR;
 	if (yych != '"') goto yy18;
 	++YYCURSOR;
 #line 668 "ext/standard/var_unserializer.re"
 	{
 	size_t len, maxlen;
 	zend_string *str;
 
 	len = parse_uiv(start + 2);
 	maxlen = max - YYCURSOR;
 	if (maxlen < len) {
 		*p = start + 2;
 		return 0;
 	}
 
 	if ((str = unserialize_str(&YYCURSOR, len, maxlen)) == NULL) {
 		return 0;
 	}
 
 	if (*(YYCURSOR) != '"') {
 		zend_string_free(str);
 		*p = YYCURSOR;
 		return 0;
 	}
 
 	if (*(YYCURSOR + 1) != ';') {
 		efree(str);
 		*p = YYCURSOR + 1;
 		return 0;
 	}
 
 	YYCURSOR += 2;
 	*p = YYCURSOR;
 
  	ZVAL_STR(rval, str);
  	return 1;
  }
#line 936 "ext/standard/var_unserializer.c"
// #line 937 "ext/standard/var_unserializer.c"
  yy46:
  	yych = *++YYCURSOR;
  	if (yych == '+') goto yy47;
 	if (yych <= '/') goto yy18;
 	if (yych <= '9') goto yy48;
 	goto yy18;
 yy47:
 	yych = *++YYCURSOR;
 	if (yych <= '/') goto yy18;
 	if (yych >= ':') goto yy18;
 yy48:
 	++YYCURSOR;
 	if ((YYLIMIT - YYCURSOR) < 2) YYFILL(2);
 	yych = *YYCURSOR;
 	if (yych <= '/') goto yy18;
 	if (yych <= '9') goto yy48;
 	if (yych >= ';') goto yy18;
 	yych = *++YYCURSOR;
 	if (yych != '"') goto yy18;
 	++YYCURSOR;
 #line 636 "ext/standard/var_unserializer.re"
 	{
 	size_t len, maxlen;
 	char *str;
 
 	len = parse_uiv(start + 2);
 	maxlen = max - YYCURSOR;
 	if (maxlen < len) {
 		*p = start + 2;
 		return 0;
 	}
 
 	str = (char*)YYCURSOR;
 
 	YYCURSOR += len;
 
 	if (*(YYCURSOR) != '"') {
 		*p = YYCURSOR;
 		return 0;
 	}
 
 	if (*(YYCURSOR + 1) != ';') {
 		*p = YYCURSOR + 1;
 		return 0;
 	}
 
 	YYCURSOR += 2;
 	*p = YYCURSOR;
 
  	ZVAL_STRINGL(rval, str, len);
  	return 1;
  }
#line 989 "ext/standard/var_unserializer.c"
// #line 990 "ext/standard/var_unserializer.c"
  yy53:
  	yych = *++YYCURSOR;
  	if (yych <= '/') {
 		if (yych <= ',') {
 			if (yych == '+') goto yy57;
 			goto yy18;
 		} else {
 			if (yych <= '-') goto yy55;
 			if (yych <= '.') goto yy60;
 			goto yy18;
 		}
 	} else {
 		if (yych <= 'I') {
 			if (yych <= '9') goto yy58;
 			if (yych <= 'H') goto yy18;
 			goto yy56;
 		} else {
 			if (yych != 'N') goto yy18;
 		}
 	}
 	yych = *++YYCURSOR;
 	if (yych == 'A') goto yy76;
 	goto yy18;
 yy55:
 	yych = *++YYCURSOR;
 	if (yych <= '/') {
 		if (yych == '.') goto yy60;
 		goto yy18;
 	} else {
 		if (yych <= '9') goto yy58;
 		if (yych != 'I') goto yy18;
 	}
 yy56:
 	yych = *++YYCURSOR;
 	if (yych == 'N') goto yy72;
 	goto yy18;
 yy57:
 	yych = *++YYCURSOR;
 	if (yych == '.') goto yy60;
 	if (yych <= '/') goto yy18;
 	if (yych >= ':') goto yy18;
 yy58:
 	++YYCURSOR;
 	if ((YYLIMIT - YYCURSOR) < 4) YYFILL(4);
 	yych = *YYCURSOR;
 	if (yych <= ':') {
 		if (yych <= '.') {
 			if (yych <= '-') goto yy18;
 			goto yy70;
 		} else {
 			if (yych <= '/') goto yy18;
 			if (yych <= '9') goto yy58;
 			goto yy18;
 		}
 	} else {
 		if (yych <= 'E') {
 			if (yych <= ';') goto yy63;
 			if (yych <= 'D') goto yy18;
 			goto yy65;
 		} else {
 			if (yych == 'e') goto yy65;
 			goto yy18;
 		}
 	}
 yy60:
 	yych = *++YYCURSOR;
 	if (yych <= '/') goto yy18;
 	if (yych >= ':') goto yy18;
 yy61:
 	++YYCURSOR;
 	if ((YYLIMIT - YYCURSOR) < 4) YYFILL(4);
 	yych = *YYCURSOR;
 	if (yych <= ';') {
 		if (yych <= '/') goto yy18;
 		if (yych <= '9') goto yy61;
 		if (yych <= ':') goto yy18;
 	} else {
 		if (yych <= 'E') {
 			if (yych <= 'D') goto yy18;
 			goto yy65;
 		} else {
 			if (yych == 'e') goto yy65;
 			goto yy18;
 		}
 	}
 yy63:
 	++YYCURSOR;
 #line 627 "ext/standard/var_unserializer.re"
 	{
 #if SIZEOF_ZEND_LONG == 4
 use_double:
 #endif
 	*p = YYCURSOR;
  	ZVAL_DOUBLE(rval, zend_strtod((const char *)start + 2, NULL));
  	return 1;
  }
#line 1086 "ext/standard/var_unserializer.c"
// #line 1087 "ext/standard/var_unserializer.c"
  yy65:
  	yych = *++YYCURSOR;
  	if (yych <= ',') {
 		if (yych != '+') goto yy18;
 	} else {
 		if (yych <= '-') goto yy66;
 		if (yych <= '/') goto yy18;
 		if (yych <= '9') goto yy67;
 		goto yy18;
 	}
 yy66:
 	yych = *++YYCURSOR;
 	if (yych <= ',') {
 		if (yych == '+') goto yy69;
 		goto yy18;
 	} else {
 		if (yych <= '-') goto yy69;
 		if (yych <= '/') goto yy18;
 		if (yych >= ':') goto yy18;
 	}
 yy67:
 	++YYCURSOR;
 	if (YYLIMIT <= YYCURSOR) YYFILL(1);
 	yych = *YYCURSOR;
 	if (yych <= '/') goto yy18;
 	if (yych <= '9') goto yy67;
 	if (yych == ';') goto yy63;
 	goto yy18;
 yy69:
 	yych = *++YYCURSOR;
 	if (yych <= '/') goto yy18;
 	if (yych <= '9') goto yy67;
 	goto yy18;
 yy70:
 	++YYCURSOR;
 	if ((YYLIMIT - YYCURSOR) < 4) YYFILL(4);
 	yych = *YYCURSOR;
 	if (yych <= ';') {
 		if (yych <= '/') goto yy18;
 		if (yych <= '9') goto yy70;
 		if (yych <= ':') goto yy18;
 		goto yy63;
 	} else {
 		if (yych <= 'E') {
 			if (yych <= 'D') goto yy18;
 			goto yy65;
 		} else {
 			if (yych == 'e') goto yy65;
 			goto yy18;
 		}
 	}
 yy72:
 	yych = *++YYCURSOR;
 	if (yych != 'F') goto yy18;
 yy73:
 	yych = *++YYCURSOR;
 	if (yych != ';') goto yy18;
 	++YYCURSOR;
 #line 611 "ext/standard/var_unserializer.re"
 	{
 	*p = YYCURSOR;
 
 	if (!strncmp((char*)start + 2, "NAN", 3)) {
 		ZVAL_DOUBLE(rval, php_get_nan());
 	} else if (!strncmp((char*)start + 2, "INF", 3)) {
 		ZVAL_DOUBLE(rval, php_get_inf());
 	} else if (!strncmp((char*)start + 2, "-INF", 4)) {
 		ZVAL_DOUBLE(rval, -php_get_inf());
 	} else {
 		ZVAL_NULL(rval);
 	}
  
  	return 1;
  }
#line 1161 "ext/standard/var_unserializer.c"
// #line 1162 "ext/standard/var_unserializer.c"
  yy76:
  	yych = *++YYCURSOR;
  	if (yych == 'N') goto yy73;
 	goto yy18;
 yy77:
 	yych = *++YYCURSOR;
 	if (yych <= ',') {
 		if (yych != '+') goto yy18;
 	} else {
 		if (yych <= '-') goto yy78;
 		if (yych <= '/') goto yy18;
 		if (yych <= '9') goto yy79;
 		goto yy18;
 	}
 yy78:
 	yych = *++YYCURSOR;
 	if (yych <= '/') goto yy18;
 	if (yych >= ':') goto yy18;
 yy79:
 	++YYCURSOR;
 	if (YYLIMIT <= YYCURSOR) YYFILL(1);
 	yych = *YYCURSOR;
 	if (yych <= '/') goto yy18;
 	if (yych <= '9') goto yy79;
 	if (yych != ';') goto yy18;
 	++YYCURSOR;
 #line 585 "ext/standard/var_unserializer.re"
 	{
 #if SIZEOF_ZEND_LONG == 4
 	int digits = YYCURSOR - start - 3;
 
 	if (start[2] == '-' || start[2] == '+') {
 		digits--;
 	}
 
 	 
 	if (digits >= MAX_LENGTH_OF_LONG - 1) {
 		if (digits == MAX_LENGTH_OF_LONG - 1) {
 			int cmp = strncmp((char*)YYCURSOR - MAX_LENGTH_OF_LONG, long_min_digits, MAX_LENGTH_OF_LONG - 1);
 
 			if (!(cmp < 0 || (cmp == 0 && start[2] == '-'))) {
 				goto use_double;
 			}
 		} else {
 			goto use_double;
 		}
 	}
 #endif
 	*p = YYCURSOR;
  	ZVAL_LONG(rval, parse_iv(start + 2));
  	return 1;
  }
#line 1214 "ext/standard/var_unserializer.c"
// #line 1215 "ext/standard/var_unserializer.c"
  yy83:
  	yych = *++YYCURSOR;
  	if (yych <= '/') goto yy18;
 	if (yych >= '2') goto yy18;
 	yych = *++YYCURSOR;
 	if (yych != ';') goto yy18;
 	++YYCURSOR;
 #line 579 "ext/standard/var_unserializer.re"
 	{
 	*p = YYCURSOR;
  	ZVAL_BOOL(rval, parse_iv(start + 2));
  	return 1;
  }
#line 1228 "ext/standard/var_unserializer.c"
// #line 1229 "ext/standard/var_unserializer.c"
  yy87:
  	++YYCURSOR;
  #line 573 "ext/standard/var_unserializer.re"
 	{
 	*p = YYCURSOR;
  	ZVAL_NULL(rval);
  	return 1;
  }
#line 1237 "ext/standard/var_unserializer.c"
// #line 1238 "ext/standard/var_unserializer.c"
  yy89:
  	yych = *++YYCURSOR;
  	if (yych <= ',') {
 		if (yych != '+') goto yy18;
 	} else {
 		if (yych <= '-') goto yy90;
 		if (yych <= '/') goto yy18;
 		if (yych <= '9') goto yy91;
 		goto yy18;
 	}
 yy90:
 	yych = *++YYCURSOR;
 	if (yych <= '/') goto yy18;
 	if (yych >= ':') goto yy18;
 yy91:
 	++YYCURSOR;
 	if (YYLIMIT <= YYCURSOR) YYFILL(1);
 	yych = *YYCURSOR;
 	if (yych <= '/') goto yy18;
 	if (yych <= '9') goto yy91;
 	if (yych != ';') goto yy18;
 	++YYCURSOR;
 #line 548 "ext/standard/var_unserializer.re"
 	{
 	zend_long id;
 
  	*p = YYCURSOR;
 	if (!var_hash) return 0;
 
 	id = parse_iv(start + 2) - 1;
 	if (id == -1 || (rval_ref = var_access(var_hash, id)) == NULL) {
 		return 0;
 	}
 
 	if (rval_ref == rval) {
 		return 0;
 	}
 
 	if (Z_ISUNDEF_P(rval_ref) || (Z_ISREF_P(rval_ref) && Z_ISUNDEF_P(Z_REFVAL_P(rval_ref)))) {
 		ZVAL_UNDEF(rval);
 		return 1;
 	}
 
 	ZVAL_COPY(rval, rval_ref);
  
  	return 1;
  }
#line 1285 "ext/standard/var_unserializer.c"
// #line 1286 "ext/standard/var_unserializer.c"
  yy95:
  	yych = *++YYCURSOR;
  	if (yych <= ',') {
 		if (yych != '+') goto yy18;
 	} else {
 		if (yych <= '-') goto yy96;
 		if (yych <= '/') goto yy18;
 		if (yych <= '9') goto yy97;
 		goto yy18;
 	}
 yy96:
 	yych = *++YYCURSOR;
 	if (yych <= '/') goto yy18;
 	if (yych >= ':') goto yy18;
 yy97:
 	++YYCURSOR;
 	if (YYLIMIT <= YYCURSOR) YYFILL(1);
 	yych = *YYCURSOR;
 	if (yych <= '/') goto yy18;
 	if (yych <= '9') goto yy97;
 	if (yych != ';') goto yy18;
 	++YYCURSOR;
 #line 522 "ext/standard/var_unserializer.re"
 	{
 	zend_long id;
 
  	*p = YYCURSOR;
 	if (!var_hash) return 0;
 
 	id = parse_iv(start + 2) - 1;
 	if (id == -1 || (rval_ref = var_access(var_hash, id)) == NULL) {
 		return 0;
 	}
 
 	zval_ptr_dtor(rval);
 	if (Z_ISUNDEF_P(rval_ref) || (Z_ISREF_P(rval_ref) && Z_ISUNDEF_P(Z_REFVAL_P(rval_ref)))) {
 		ZVAL_UNDEF(rval);
 		return 1;
 	}
 	if (Z_ISREF_P(rval_ref)) {
 		ZVAL_COPY(rval, rval_ref);
 	} else {
 		ZVAL_NEW_REF(rval_ref, rval_ref);
 		ZVAL_COPY(rval, rval_ref);
 	}
  
  	return 1;
  }
#line 1334 "ext/standard/var_unserializer.c"
// #line 1335 "ext/standard/var_unserializer.c"
  }
  #line 886 "ext/standard/var_unserializer.re"
  
 
 	return 0;
 }