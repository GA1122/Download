get_standard_integer_type(const char *l, const char **t)
{
	int type;

	if (isalpha((unsigned char)l[1])) {
		switch (l[1]) {
		case 'C':
			 
			type = FILE_BYTE;
			break;
		case 'S':
			 
			type = FILE_SHORT;
			break;
		case 'I':
		case 'L':
			 
			type = FILE_LONG;
			break;
		case 'Q':
			 
			type = FILE_QUAD;
			break;
		default:
			 
			return FILE_INVALID;
		}
		l += 2;
	} else if (isdigit((unsigned char)l[1])) {
		 
		if (isdigit((unsigned char)l[2])) {
			 
			return FILE_INVALID;
		}
		switch (l[1]) {
		case '1':
			type = FILE_BYTE;
			break;
		case '2':
			type = FILE_SHORT;
			break;
		case '4':
			type = FILE_LONG;
			break;
		case '8':
			type = FILE_QUAD;
			break;
		default:
			 
			return FILE_INVALID;
		}
		l += 2;
	} else {
		 
		type = FILE_LONG;
		++l;
	}
	if (t)
		*t = l;
	return type;
}
