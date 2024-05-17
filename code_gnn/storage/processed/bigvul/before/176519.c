xmlParse3986Host(xmlURIPtr uri, const char **str)
{
 const char *cur = *str;
 const char *host;

    host = cur;
  
 if (*cur == '[') {
        cur++;
 while ((*cur != ']') && (*cur != 0))
	    cur++;
 if (*cur != ']')
 return(1);
	cur++;
 goto found;
 }
  
 if (ISA_DIGIT(cur)) {
 if (xmlParse3986DecOctet(&cur) != 0)
 goto not_ipv4;
 if (*cur != '.')
 goto not_ipv4;
	cur++;
 if (xmlParse3986DecOctet(&cur) != 0)
 goto not_ipv4;
 if (*cur != '.')
 goto not_ipv4;
 if (xmlParse3986DecOctet(&cur) != 0)
 goto not_ipv4;
 if (*cur != '.')
 goto not_ipv4;
 if (xmlParse3986DecOctet(&cur) != 0)
 goto not_ipv4;
 goto found;
not_ipv4:
        cur = *str;
 }
  
 while (ISA_UNRESERVED(cur) || ISA_PCT_ENCODED(cur) || ISA_SUB_DELIM(cur))
        NEXT(cur);
found:
 if (uri != NULL) {
 if (uri->authority != NULL) xmlFree(uri->authority);
	uri->authority = NULL;
 if (uri->server != NULL) xmlFree(uri->server);
 if (cur != host) {
 if (uri->cleanup & 2)
		uri->server = STRNDUP(host, cur - host);
 else
		uri->server = xmlURIUnescapeString(host, cur - host, NULL);
 } else
	    uri->server = NULL;
 }
 *str = cur;
 return(0);
}
