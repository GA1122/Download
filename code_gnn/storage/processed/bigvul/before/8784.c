static const char *valid_domain_label(const char *label)
{
	unsigned char ch;

	if (label[0] == '-')
		return NULL;
	for (;;) {
		ch = *label;
		if ((ch|0x20) < 'a' || (ch|0x20) > 'z') {
			if (ch < '0' || ch > '9') {
				if (ch == '\0' || ch == '.')
					return label;
				 
				if (ch != '-' && ch != '_')
					return NULL;
			}
		}
		label++;
	}
}
