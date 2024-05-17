handleMailto(char *url)
{
    Str to;
    char *pos;

    if (strncasecmp(url, "mailto:", 7))
	return 0;
#ifdef USE_W3MMAILER
    if (! non_null(Mailer) || MailtoOptions == MAILTO_OPTIONS_USE_W3MMAILER)
	return 0;
#else
    if (!non_null(Mailer)) {
	 
	disp_err_message("no mailer is specified", TRUE);
	return 1;
    }
#endif
	
     
    if (MailtoOptions == MAILTO_OPTIONS_USE_MAILTO_URL) {
	to = Strnew_charp(html_unquote(url));
    } else {
	to = Strnew_charp(url + 7);
	if ((pos = strchr(to->ptr, '?')) != NULL)
	    Strtruncate(to, pos - to->ptr);
    }
    fmTerm();
    system(myExtCommand(Mailer, shell_quote(file_unquote(to->ptr)),
			FALSE)->ptr);
    fmInit();
    displayBuffer(Currentbuf, B_FORCE_REDRAW);
    pushHashHist(URLHist, url);
    return 1;
}