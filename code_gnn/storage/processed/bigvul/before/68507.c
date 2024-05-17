server_httpmethod_cmp(const void *a, const void *b)
{
	const struct http_method *ma = a;
	const struct http_method *mb = b;

	 
	return (strcmp(ma->method_name, mb->method_name));
}
