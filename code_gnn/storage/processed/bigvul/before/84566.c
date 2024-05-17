parse_proxy()
{
    if (non_null(HTTP_proxy))
	parseURL(HTTP_proxy, &HTTP_proxy_parsed, NULL);
#ifdef USE_SSL
    if (non_null(HTTPS_proxy))
	parseURL(HTTPS_proxy, &HTTPS_proxy_parsed, NULL);
#endif				 
#ifdef USE_GOPHER
    if (non_null(GOPHER_proxy))
	parseURL(GOPHER_proxy, &GOPHER_proxy_parsed, NULL);
#endif
    if (non_null(FTP_proxy))
	parseURL(FTP_proxy, &FTP_proxy_parsed, NULL);
    if (non_null(NO_proxy))
	set_no_proxy(NO_proxy);
}