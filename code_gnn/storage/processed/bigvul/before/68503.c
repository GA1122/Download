server_httperror_byid(unsigned int id)
{
	struct http_error	 error, *res;

	 
	error.error_code = (int)id;

	if ((res = bsearch(&error, http_errors,
	    sizeof(http_errors) / sizeof(http_errors[0]) - 1,
	    sizeof(http_errors[0]), server_httperror_cmp)) != NULL)
		return (res->error_name);

	return (NULL);
}