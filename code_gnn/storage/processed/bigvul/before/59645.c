uripMatch(const char * URI) {
    if ((URI == NULL) || (!strcmp(URI, "file:///etc/xml/catalog")))
        return(0);
     
    if (strcmp(urip_rcvsURLs[urip_current], URI))
	urip_success = 0;
    return(1);
}