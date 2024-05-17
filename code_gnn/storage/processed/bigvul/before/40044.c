PGTYPESdate_new(void)
{
	date	   *result;

	result = (date *) pgtypes_alloc(sizeof(date));
	 
	return result;
}
