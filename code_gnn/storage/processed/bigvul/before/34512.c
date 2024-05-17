static char * check_special_flags (char * sfs, Node * e)
{
	char * p = sfs;
	int cont = 1;

	 
	while (cont) {
		switch (*p) {
			case 'P':
				p++;
				e->flags |= MISC_FMT_PRESERVE_ARGV0;
				break;
			case 'O':
				p++;
				e->flags |= MISC_FMT_OPEN_BINARY;
				break;
			case 'C':
				p++;
				 
				e->flags |= (MISC_FMT_CREDENTIALS |
						MISC_FMT_OPEN_BINARY);
				break;
			default:
				cont = 0;
		}
	}

	return p;
}
