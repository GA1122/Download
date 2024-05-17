goURL0(char *prompt, int relative)
{
    char *url, *referer;
    ParsedURL p_url, *current;
    Buffer *cur_buf = Currentbuf;
    const int *no_referer_ptr;

    url = searchKeyData();
    if (url == NULL) {
	Hist *hist = copyHist(URLHist);
	Anchor *a;

	current = baseURL(Currentbuf);
	if (current) {
	    char *c_url = parsedURL2Str(current)->ptr;
	    if (DefaultURLString == DEFAULT_URL_CURRENT)
		url = url_decode2(c_url, NULL);
	    else
		pushHist(hist, c_url);
	}
	a = retrieveCurrentAnchor(Currentbuf);
	if (a) {
	    char *a_url;
	    parseURL2(a->url, &p_url, current);
	    a_url = parsedURL2Str(&p_url)->ptr;
	    if (DefaultURLString == DEFAULT_URL_LINK)
		url = url_decode2(a_url, Currentbuf);
	    else
		pushHist(hist, a_url);
	}
	url = inputLineHist(prompt, url, IN_URL, hist);
	if (url != NULL)
	    SKIP_BLANKS(url);
    }
    if (relative) {
	no_referer_ptr = query_SCONF_NO_REFERER_FROM(&Currentbuf->currentURL);
	current = baseURL(Currentbuf);
	if ((no_referer_ptr && *no_referer_ptr) ||
	    current == NULL ||
	    current->scheme == SCM_LOCAL || current->scheme == SCM_LOCAL_CGI)
	    referer = NO_REFERER;
	else
	    referer = parsedURL2Str(&Currentbuf->currentURL)->ptr;
	url = url_encode(url, current, Currentbuf->document_charset);
    }
    else {
	current = NULL;
	referer = NULL;
	url = url_encode(url, NULL, 0);
    }
    if (url == NULL || *url == '\0') {
	displayBuffer(Currentbuf, B_FORCE_REDRAW);
	return;
    }
    if (*url == '#') {
	gotoLabel(url + 1);
	return;
    }
    parseURL2(url, &p_url, current);
    pushHashHist(URLHist, parsedURL2Str(&p_url)->ptr);
    cmd_loadURL(url, current, referer, NULL);
    if (Currentbuf != cur_buf)	 
	pushHashHist(URLHist, parsedURL2Str(&Currentbuf->currentURL)->ptr);
}