DEFUN(vwSrc, SOURCE VIEW, "Toggle between HTML shown or processed")
{
    Buffer *buf;

    if (Currentbuf->type == NULL || Currentbuf->bufferprop & BP_FRAME)
	return;
    if ((buf = Currentbuf->linkBuffer[LB_SOURCE]) != NULL ||
	(buf = Currentbuf->linkBuffer[LB_N_SOURCE]) != NULL) {
	Currentbuf = buf;
	displayBuffer(Currentbuf, B_NORMAL);
	return;
    }
    if (Currentbuf->sourcefile == NULL) {
	if (Currentbuf->pagerSource &&
	    !strcasecmp(Currentbuf->type, "text/plain")) {
#ifdef USE_M17N
	    wc_ces old_charset;
	    wc_bool old_fix_width_conv;
#endif
	    FILE *f;
	    Str tmpf = tmpfname(TMPF_SRC, NULL);
	    f = fopen(tmpf->ptr, "w");
	    if (f == NULL)
		return;
#ifdef USE_M17N
	    old_charset = DisplayCharset;
	    old_fix_width_conv = WcOption.fix_width_conv;
	    DisplayCharset = (Currentbuf->document_charset != WC_CES_US_ASCII)
		? Currentbuf->document_charset : 0;
	    WcOption.fix_width_conv = WC_FALSE;
#endif
	    saveBufferBody(Currentbuf, f, TRUE);
#ifdef USE_M17N
	    DisplayCharset = old_charset;
	    WcOption.fix_width_conv = old_fix_width_conv;
#endif
	    fclose(f);
	    Currentbuf->sourcefile = tmpf->ptr;
	}
	else {
	    return;
	}
    }

    buf = newBuffer(INIT_BUFFER_WIDTH);

    if (is_html_type(Currentbuf->type)) {
	buf->type = "text/plain";
	if (Currentbuf->real_type &&
	    is_html_type(Currentbuf->real_type))
	    buf->real_type = "text/plain";
	else
	    buf->real_type = Currentbuf->real_type;
	buf->buffername = Sprintf("source of %s", Currentbuf->buffername)->ptr;
	buf->linkBuffer[LB_N_SOURCE] = Currentbuf;
	Currentbuf->linkBuffer[LB_SOURCE] = buf;
    }
    else if (!strcasecmp(Currentbuf->type, "text/plain")) {
	buf->type = "text/html";
	if (Currentbuf->real_type &&
	    !strcasecmp(Currentbuf->real_type, "text/plain"))
	    buf->real_type = "text/html";
	else
	    buf->real_type = Currentbuf->real_type;
	buf->buffername = Sprintf("HTML view of %s",
				  Currentbuf->buffername)->ptr;
	buf->linkBuffer[LB_SOURCE] = Currentbuf;
	Currentbuf->linkBuffer[LB_N_SOURCE] = buf;
    }
    else {
	return;
    }
    buf->currentURL = Currentbuf->currentURL;
    buf->real_scheme = Currentbuf->real_scheme;
    buf->filename = Currentbuf->filename;
    buf->sourcefile = Currentbuf->sourcefile;
    buf->header_source = Currentbuf->header_source;
    buf->search_header = Currentbuf->search_header;
#ifdef USE_M17N
    buf->document_charset = Currentbuf->document_charset;
#endif
    buf->clone = Currentbuf->clone;
    (*buf->clone)++;

    buf->need_reshape = TRUE;
    reshapeBuffer(buf);
    pushBuffer(buf);
    displayBuffer(Currentbuf, B_NORMAL);
}
