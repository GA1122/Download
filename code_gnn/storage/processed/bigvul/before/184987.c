 xmlParseNCNameComplex(xmlParserCtxtPtr ctxt) {
     int len = 0, l;
     int c;
     int count = 0;
 
 #ifdef DEBUG
     nbParseNCNameComplex++;
 #endif
 
      
     GROW;
     c = CUR_CHAR(l);
     if ((c == ' ') || (c == '>') || (c == '/') ||  
 	(!xmlIsNameStartChar(ctxt, c) || (c == ':'))) {
 	return(NULL);
     }
 
     while ((c != ' ') && (c != '>') && (c != '/') &&  
 	   (xmlIsNameChar(ctxt, c) && (c != ':'))) {
  	if (count++ > 100) {
  	    count = 0;
  	    GROW;
//             if (ctxt->instate == XML_PARSER_EOF)
//                 return(NULL);
  	}
  	len += l;
  	NEXTL(l);
 	c = CUR_CHAR(l);
     }
     return(xmlDictLookup(ctxt->dict, ctxt->input->cur - len, len));
 }