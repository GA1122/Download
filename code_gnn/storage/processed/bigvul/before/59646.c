uripOpen(const char * URI) {
    if ((URI == NULL) || (!strcmp(URI, "file:///etc/xml/catalog")))
        return(NULL);
     
    if (strcmp(urip_rcvsURLs[urip_current], URI))
	urip_success = 0;
    urip_cur = urip_res;
    urip_rlen = strlen(urip_res);
    return((void *) urip_cur);
}