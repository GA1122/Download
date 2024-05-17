server_http_time(time_t t, char *tmbuf, size_t len)
{
	struct tm		 tm;

	 
	if (t == -1 || gmtime_r(&t, &tm) == NULL)
		return (-1);
	else
		return (strftime(tmbuf, len, "%a, %d %h %Y %T %Z", &tm));
}
