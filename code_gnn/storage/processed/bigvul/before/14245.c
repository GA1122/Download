static int _php_libxml_free_error(xmlErrorPtr error)
{
	 
	xmlResetError(error);
	return 1;
}
