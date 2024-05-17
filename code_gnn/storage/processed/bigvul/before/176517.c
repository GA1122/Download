xmlParse3986Fragment(xmlURIPtr uri, const char **str)
{
 const char *cur;

 if (str == NULL)
 return (-1);

    cur = *str;

 while ((ISA_PCHAR(cur)) || (*cur == '/') || (*cur == '?') ||
 (*cur == '[') || (*cur == ']') ||
 ((uri != NULL) && (uri->cleanup & 1) && (IS_UNWISE(cur))))
        NEXT(cur);
 if (uri != NULL) {
 if (uri->fragment != NULL)
            xmlFree(uri->fragment);
 if (uri->cleanup & 2)
	    uri->fragment = STRNDUP(*str, cur - *str);
 else
	    uri->fragment = xmlURIUnescapeString(*str, cur - *str, NULL);
 }
 *str = cur;
 return (0);
}
