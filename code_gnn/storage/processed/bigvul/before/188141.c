 xmlParse3986Port(xmlURIPtr uri, const char **str)
 {
  const char *cur = *str;
  unsigned port = 0;  
 
  if (ISA_DIGIT(cur)) {
  while (ISA_DIGIT(cur)) {
 	    port = port * 10 + (*cur - '0');
 
 
  	    cur++;
  	}
  	if (uri != NULL)
	    uri->port = port & INT_MAX;  
// 	    uri->port = port & USHRT_MAX;  
  	*str = cur;
  	return(0);
      }
  return(1);
 }