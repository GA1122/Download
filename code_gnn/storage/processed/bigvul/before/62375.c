isnonzero(const u_char *a, size_t len)
{
	while (len > 0) {
		if (*a != 0)
			return (1);
		a++;
		len--;
	}
 	return (0);
 }