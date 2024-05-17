int expand_escape(const char **data)
{
        char digit[4];

	switch (**data) {
	case 't':
		return '\t';
	case 'r':
		return '\r';
	case 'n':
		return '\n';
	case 'e':
		return 27;  
	case '\\':
		return '\\';

	case 'x':
                 
		if (!i_isxdigit((*data)[1]) || !i_isxdigit((*data)[2]))
			return -1;

		digit[0] = (*data)[1];
		digit[1] = (*data)[2];
                digit[2] = '\0';
		*data += 2;
		return strtol(digit, NULL, 16);
	case 'c':
                 
                (*data)++;
		return i_toupper(**data) - 64;
	case '0': case '1': case '2': case '3':
	case '4': case '5': case '6': case '7':
                 
		digit[1] = digit[2] = digit[3] = '\0';
                digit[0] = (*data)[0];
		if ((*data)[1] >= '0' && (*data)[1] <= '7') {
			++*data;
			digit[1] = **data;
			if ((*data)[1] >= '0' && (*data)[1] <= '7') {
				++*data;
				digit[2] = **data;
			}
		}
		return strtol(digit, NULL, 8);
	default:
		return -1;
	}
}