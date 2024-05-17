xmlBuildRelativeURI (const xmlChar * URI, const xmlChar * base)
{
    xmlChar *val = NULL;
 int ret;
 int ix;
 int pos = 0;
 int nbslash = 0;
 int len;
    xmlURIPtr ref = NULL;
    xmlURIPtr bas = NULL;
    xmlChar *bptr, *uptr, *vptr;
 int remove_path = 0;

 if ((URI == NULL) || (*URI == 0))
 return NULL;

  
    ref = xmlCreateURI ();
 if (ref == NULL)
 return NULL;
  
 if (URI[0] != '.') {
	ret = xmlParseURIReference (ref, (const char *) URI);
 if (ret != 0)
 goto done;  
 } else
	ref->path = (char *)xmlStrdup(URI);

  
 if ((base == NULL) || (*base == 0)) {
	val = xmlStrdup (URI);
 goto done;
 }
    bas = xmlCreateURI ();
 if (bas == NULL)
 goto done;
 if (base[0] != '.') {
	ret = xmlParseURIReference (bas, (const char *) base);
 if (ret != 0)
 goto done;  
 } else
	bas->path = (char *)xmlStrdup(base);

  
 if ((ref->scheme != NULL) &&
 ((bas->scheme == NULL) ||
 (xmlStrcmp ((xmlChar *)bas->scheme, (xmlChar *)ref->scheme)) ||
 (xmlStrcmp ((xmlChar *)bas->server, (xmlChar *)ref->server)))) {
	val = xmlStrdup (URI);
 goto done;
 }
 if (xmlStrEqual((xmlChar *)bas->path, (xmlChar *)ref->path)) {
	val = xmlStrdup(BAD_CAST "");
 goto done;
 }
 if (bas->path == NULL) {
	val = xmlStrdup((xmlChar *)ref->path);
 goto done;
 }
 if (ref->path == NULL) {
        ref->path = (char *) "/";
	remove_path = 1;
 }

  
 if (bas->path == NULL) {
 if (ref->path != NULL) {
	    uptr = (xmlChar *) ref->path;
 if (*uptr == '/')
		uptr++;
  
	    val = xmlURIEscapeStr(uptr, BAD_CAST "/;&=+$,");
 }
 goto done;
 }
    bptr = (xmlChar *)bas->path;
 if (ref->path == NULL) {
 for (ix = 0; bptr[ix] != 0; ix++) {
 if (bptr[ix] == '/')
		nbslash++;
 }
	uptr = NULL;
	len = 1;  
 } else {
  
 if ((ref->path[pos] == '.') && (ref->path[pos+1] == '/'))
            pos += 2;
 if ((*bptr == '.') && (bptr[1] == '/'))
            bptr += 2;
 else if ((*bptr == '/') && (ref->path[pos] != '/'))
	    bptr++;
 while ((bptr[pos] == ref->path[pos]) && (bptr[pos] != 0))
	    pos++;

 if (bptr[pos] == ref->path[pos]) {
	    val = xmlStrdup(BAD_CAST "");
 goto done;  
 }

  
	ix = pos;
 if ((ref->path[ix] == '/') && (ix > 0))
	    ix--;
 else if ((ref->path[ix] == 0) && (ix > 1) && (ref->path[ix - 1] == '/'))
	    ix -= 2;
 for (; ix > 0; ix--) {
 if (ref->path[ix] == '/')
 break;
 }
 if (ix == 0) {
	    uptr = (xmlChar *)ref->path;
 } else {
	    ix++;
	    uptr = (xmlChar *)&ref->path[ix];
 }

  
 if (bptr[pos] != ref->path[pos]) { 
 for (; bptr[ix] != 0; ix++) {
 if (bptr[ix] == '/')
		    nbslash++;
 }
 }
	len = xmlStrlen (uptr) + 1;
 }

 if (nbslash == 0) {
 if (uptr != NULL)
  
	    val = xmlURIEscapeStr(uptr, BAD_CAST "/;&=+$,");
 goto done;
 }

  
    val = (xmlChar *) xmlMalloc (len + 3 * nbslash);
 if (val == NULL) {
        xmlURIErrMemory("building relative URI\n");
 goto done;
 }
    vptr = val;
  
 for (; nbslash>0; nbslash--) {
 *vptr++ = '.';
 *vptr++ = '.';
 *vptr++ = '/';
 }
  
 if (uptr != NULL) {
 if ((vptr > val) && (len > 0) &&
 (uptr[0] == '/') && (vptr[-1] == '/')) {
	    memcpy (vptr, uptr + 1, len - 1);
	    vptr[len - 2] = 0;
 } else {
	    memcpy (vptr, uptr, len);
	    vptr[len - 1] = 0;
 }
 } else {
	vptr[len - 1] = 0;
 }

  
    vptr = val;
  
    val = xmlURIEscapeStr(vptr, BAD_CAST "/;&=+$,");
    xmlFree(vptr);

done:
  
 if (remove_path != 0)
        ref->path = NULL;
 if (ref != NULL)
	xmlFreeURI (ref);
 if (bas != NULL)
	xmlFreeURI (bas);

 return val;
}
