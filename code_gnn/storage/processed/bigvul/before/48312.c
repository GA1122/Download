add_ms(tmsize_t m1, tmsize_t m2)
{
	tmsize_t bytes = m1 + m2;

	 
	if (m1 == 0 || m2 == 0)
		bytes = 0;
	else if (bytes <= m1 || bytes <= m2)
		bytes = 0;

	return bytes;
}