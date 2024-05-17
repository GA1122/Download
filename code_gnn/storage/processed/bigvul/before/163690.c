xmlBufWriteQuotedString(xmlBufPtr buf, const xmlChar *string) {
    const xmlChar *cur, *base;
    if ((buf == NULL) || (buf->error))
        return(-1);
    CHECK_COMPAT(buf)
    if (buf->alloc == XML_BUFFER_ALLOC_IMMUTABLE)
        return(-1);
    if (xmlStrchr(string, '\"')) {
        if (xmlStrchr(string, '\'')) {
#ifdef DEBUG_BUFFER
	    xmlGenericError(xmlGenericErrorContext,
 "xmlBufWriteQuotedString: string contains quote and double-quotes !\n");
#endif
	    xmlBufCCat(buf, "\"");
            base = cur = string;
            while(*cur != 0){
                if(*cur == '"'){
                    if (base != cur)
                        xmlBufAdd(buf, base, cur - base);
                    xmlBufAdd(buf, BAD_CAST "&quot;", 6);
                    cur++;
                    base = cur;
                }
                else {
                    cur++;
                }
            }
            if (base != cur)
                xmlBufAdd(buf, base, cur - base);
	    xmlBufCCat(buf, "\"");
	}
        else{
	    xmlBufCCat(buf, "\'");
            xmlBufCat(buf, string);
	    xmlBufCCat(buf, "\'");
        }
    } else {
        xmlBufCCat(buf, "\"");
        xmlBufCat(buf, string);
        xmlBufCCat(buf, "\"");
    }
    return(0);
}
