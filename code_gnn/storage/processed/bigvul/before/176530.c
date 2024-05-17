xmlParse3986Userinfo(xmlURIPtr uri, const char **str)
{
 const char *cur;

    cur = *str;
 while (ISA_UNRESERVED(cur) || ISA_PCT_ENCODED(cur) ||
           ISA_SUB_DELIM(cur) || (*cur == ':'))
	NEXT(cur);
 if (*cur == '@') {
 if (uri != NULL) {
 if (uri->user != NULL) xmlFree(uri->user);
 if (uri->cleanup & 2)
		uri->user = STRNDUP(*str, cur - *str);
 else
		uri->user = xmlURIUnescapeString(*str, cur - *str, NULL);
 }
 *str = cur;
 return(0);
 }
 return(1);
}
