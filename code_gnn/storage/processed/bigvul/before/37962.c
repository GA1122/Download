check_format_type(const char *ptr, int type)
{
	int quad = 0, h;
	if (*ptr == '\0') {
		 
		return -1;
	}

	switch (file_formats[type]) {
	case FILE_FMT_QUAD:
		quad = 1;
		 
	case FILE_FMT_NUM:
		if (quad == 0) {
			switch (type) {
			case FILE_BYTE:
				h = 2;
				break;
			case FILE_SHORT:
			case FILE_BESHORT:
			case FILE_LESHORT:
				h = 1;
				break;
			case FILE_LONG:
			case FILE_BELONG:
			case FILE_LELONG:
			case FILE_MELONG:
			case FILE_LEID3:
			case FILE_BEID3:
			case FILE_INDIRECT:
				h = 0;
				break;
			default:
				abort();
			}
		} else
			h = 0;
		if (*ptr == '-')
			ptr++;
		if (*ptr == '.')
			ptr++;
		while (isdigit((unsigned char)*ptr)) ptr++;
		if (*ptr == '.')
			ptr++;
		while (isdigit((unsigned char)*ptr)) ptr++;
		if (quad) {
			if (*ptr++ != 'l')
				return -1;
			if (*ptr++ != 'l')
				return -1;
		}
	
		switch (*ptr++) {
#ifdef STRICT_FORMAT 	 
		 
		case 'l':
			switch (*ptr++) {
			case 'i':
			case 'd':
			case 'u':
			case 'o':
			case 'x':
			case 'X':
				return h != 0 ? -1 : 0;
			default:
				return -1;
			}
		
		 
		case 'h':
			if (h-- <= 0)
				return -1;
			switch (*ptr++) {
			case 'h':
				if (h-- <= 0)
					return -1;
				switch (*ptr++) {
				case 'i':
				case 'd':
				case 'u':
				case 'o':
				case 'x':
				case 'X':
					return 0;
				default:
					return -1;
				}
			case 'i':
			case 'd':
			case 'u':
			case 'o':
			case 'x':
			case 'X':
				return h != 0 ? -1 : 0;
			default:
				return -1;
			}
#endif
		case 'c':
			return h != 2 ? -1 : 0;
		case 'i':
		case 'd':
		case 'u':
		case 'o':
		case 'x':
		case 'X':
#ifdef STRICT_FORMAT
			return h != 0 ? -1 : 0;
#else
			return 0;
#endif
		default:
			return -1;
		}
		
	case FILE_FMT_FLOAT:
	case FILE_FMT_DOUBLE:
		if (*ptr == '-')
			ptr++;
		if (*ptr == '.')
			ptr++;
		while (isdigit((unsigned char)*ptr)) ptr++;
		if (*ptr == '.')
			ptr++;
		while (isdigit((unsigned char)*ptr)) ptr++;
	
		switch (*ptr++) {
		case 'e':
		case 'E':
		case 'f':
		case 'F':
		case 'g':
		case 'G':
			return 0;
			
		default:
			return -1;
		}
		

	case FILE_FMT_STR:
		if (*ptr == '-')
			ptr++;
		while (isdigit((unsigned char )*ptr))
			ptr++;
		if (*ptr == '.') {
			ptr++;
			while (isdigit((unsigned char )*ptr))
				ptr++;
		}
		
		switch (*ptr++) {
		case 's':
			return 0;
		default:
			return -1;
		}
		
	default:
		 
		abort();
	}
	 
	return -1;
}