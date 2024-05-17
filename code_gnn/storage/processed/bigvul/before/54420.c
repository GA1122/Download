static void php_zip_entry_get_info(INTERNAL_FUNCTION_PARAMETERS, int opt)  
{
	zval * zip_entry;
	zip_read_rsrc * zr_rsrc;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zip_entry) == FAILURE) {
		return;
	}

	if ((zr_rsrc = (zip_read_rsrc *)zend_fetch_resource(Z_RES_P(zip_entry), le_zip_entry_name, le_zip_entry)) == NULL) {
		RETURN_FALSE;
	}

	if (!zr_rsrc->zf) {
		RETURN_FALSE;
	}

	switch (opt) {
		case 0:
			RETURN_STRING((char *)zr_rsrc->sb.name);
			break;
		case 1:
			RETURN_LONG((zend_long) (zr_rsrc->sb.comp_size));
			break;
		case 2:
			RETURN_LONG((zend_long) (zr_rsrc->sb.size));
			break;
		case 3:
			switch (zr_rsrc->sb.comp_method) {
				case 0:
					RETURN_STRING("stored");
					break;
				case 1:
					RETURN_STRING("shrunk");
					break;
				case 2:
				case 3:
				case 4:
				case 5:
					RETURN_STRING("reduced");
					break;
				case 6:
					RETURN_STRING("imploded");
					break;
				case 7:
					RETURN_STRING("tokenized");
					break;
				case 8:
					RETURN_STRING("deflated");
					break;
				case 9:
					RETURN_STRING("deflatedX");
					break;
				case 10:
					RETURN_STRING("implodedX");
					break;
				default:
					RETURN_FALSE;
			}
			RETURN_LONG((zend_long) (zr_rsrc->sb.comp_method));
			break;
	}

}
 
