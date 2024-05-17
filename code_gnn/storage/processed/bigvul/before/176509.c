xmlBuildURI(const xmlChar *URI, const xmlChar *base) {
    xmlChar *val = NULL;
 int ret, len, indx, cur, out;
    xmlURIPtr ref = NULL;
    xmlURIPtr bas = NULL;
    xmlURIPtr res = NULL;

  
 if (URI == NULL)
	ret = -1;
 else {
 if (*URI) {
	    ref = xmlCreateURI();
 if (ref == NULL)
 goto done;
	    ret = xmlParseURIReference(ref, (const char *) URI);
 }
 else
	    ret = 0;
 }
 if (ret != 0)
 goto done;
 if ((ref != NULL) && (ref->scheme != NULL)) {
  
	val = xmlStrdup(URI);
 goto done;
 }
 if (base == NULL)
	ret = -1;
 else {
	bas = xmlCreateURI();
 if (bas == NULL)
 goto done;
	ret = xmlParseURIReference(bas, (const char *) base);
 }
 if (ret != 0) {
 if (ref)
	    val = xmlSaveUri(ref);
 goto done;
 }
 if (ref == NULL) {
  
 if (bas->fragment != NULL) {
	    xmlFree(bas->fragment);
	    bas->fragment = NULL;
 }
	val = xmlSaveUri(bas);
 goto done;
 }

  
    res = xmlCreateURI();
 if (res == NULL)
 goto done;
 if ((ref->scheme == NULL) && (ref->path == NULL) &&
 ((ref->authority == NULL) && (ref->server == NULL))) {
 if (bas->scheme != NULL)
	    res->scheme = xmlMemStrdup(bas->scheme);
 if (bas->authority != NULL)
	    res->authority = xmlMemStrdup(bas->authority);
 else if (bas->server != NULL) {
	    res->server = xmlMemStrdup(bas->server);
 if (bas->user != NULL)
		res->user = xmlMemStrdup(bas->user);
	    res->port = bas->port;
 }
 if (bas->path != NULL)
	    res->path = xmlMemStrdup(bas->path);
 if (ref->query_raw != NULL)
	    res->query_raw = xmlMemStrdup (ref->query_raw);
 else if (ref->query != NULL)
	    res->query = xmlMemStrdup(ref->query);
 else if (bas->query_raw != NULL)
	    res->query_raw = xmlMemStrdup(bas->query_raw);
 else if (bas->query != NULL)
	    res->query = xmlMemStrdup(bas->query);
 if (ref->fragment != NULL)
	    res->fragment = xmlMemStrdup(ref->fragment);
 goto step_7;
 }

  
 if (ref->scheme != NULL) {
	val = xmlSaveUri(ref);
 goto done;
 }
 if (bas->scheme != NULL)
	res->scheme = xmlMemStrdup(bas->scheme);

 if (ref->query_raw != NULL)
	res->query_raw = xmlMemStrdup(ref->query_raw);
 else if (ref->query != NULL)
	res->query = xmlMemStrdup(ref->query);
 if (ref->fragment != NULL)
	res->fragment = xmlMemStrdup(ref->fragment);

  
 if ((ref->authority != NULL) || (ref->server != NULL)) {
 if (ref->authority != NULL)
	    res->authority = xmlMemStrdup(ref->authority);
 else {
	    res->server = xmlMemStrdup(ref->server);
 if (ref->user != NULL)
		res->user = xmlMemStrdup(ref->user);
            res->port = ref->port;
 }
 if (ref->path != NULL)
	    res->path = xmlMemStrdup(ref->path);
 goto step_7;
 }
 if (bas->authority != NULL)
	res->authority = xmlMemStrdup(bas->authority);
 else if (bas->server != NULL) {
	res->server = xmlMemStrdup(bas->server);
 if (bas->user != NULL)
	    res->user = xmlMemStrdup(bas->user);
	res->port = bas->port;
 }

  
 if ((ref->path != NULL) && (ref->path[0] == '/')) {
	res->path = xmlMemStrdup(ref->path);
 goto step_7;
 }


  
    len = 2;  
 if (ref->path != NULL)
	len += strlen(ref->path);
 if (bas->path != NULL)
	len += strlen(bas->path);
    res->path = (char *) xmlMallocAtomic(len);
 if (res->path == NULL) {
        xmlURIErrMemory("resolving URI against base\n");
 goto done;
 }
    res->path[0] = 0;

  
    cur = 0;
    out = 0;
 if (bas->path != NULL) {
 while (bas->path[cur] != 0) {
 while ((bas->path[cur] != 0) && (bas->path[cur] != '/'))
		cur++;
 if (bas->path[cur] == 0)
 break;

	    cur++;
 while (out < cur) {
		res->path[out] = bas->path[out];
		out++;
 }
 }
 }
    res->path[out] = 0;

  
 if (ref->path != NULL && ref->path[0] != 0) {
	indx = 0;
  
 if ((out == 0) && (bas->server != NULL))
	    res->path[out++] = '/';
 while (ref->path[indx] != 0) {
	    res->path[out++] = ref->path[indx++];
 }
 }
    res->path[out] = 0;

  
    xmlNormalizeURIPath(res->path);

step_7:

  
    val = xmlSaveUri(res);

done:
 if (ref != NULL)
	xmlFreeURI(ref);
 if (bas != NULL)
	xmlFreeURI(bas);
 if (res != NULL)
	xmlFreeURI(res);
 return(val);
}
