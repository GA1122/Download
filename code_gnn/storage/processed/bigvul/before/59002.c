HTStream *HTMLToC(HTPresentation *pres GCC_UNUSED,
		  HTParentAnchor *anchor,
		  HTStream *sink)
{
    HTStructured *html;

    if (sink)
	(*sink->isa->put_string) (sink, "/* ");		 
    html = HTML_new(anchor, WWW_PLAINTEXT, sink);
    html->comment_start = "/* ";
    html->comment_end = " */\n";	 
    if (!sink)
	HTML_put_string(html, html->comment_start);
    CTRACE((tfp, "HTMLToC calling CacheThru_new\n"));
    return CacheThru_new(anchor,
			 SGML_new(&HTML_dtd, anchor, html));
}
