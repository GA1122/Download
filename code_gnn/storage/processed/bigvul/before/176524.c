xmlParse3986Query(xmlURIPtr uri, const char **str)
{
 const char *cur;

 if (str == NULL)
 return (-1);

    cur = *str;

 while ((ISA_PCHAR(cur)) || (*cur == '/') || (*cur == '?') ||
 ((uri != NULL) && (uri->cleanup & 1) && (IS_UNWISE(cur))))
        NEXT(cur);
 if (uri != NULL) {
 if (uri->query != NULL)
            xmlFree(uri->query);
 if (uri->cleanup & 2)
	    uri->query = STRNDUP(*str, cur - *str);
 else
	    uri->query = xmlURIUnescapeString(*str, cur - *str, NULL);

  
 if (uri->query_raw != NULL)
	    xmlFree (uri->query_raw);
	uri->query_raw = STRNDUP (*str, cur - *str);
 }
 *str = cur;
 return (0);
}
