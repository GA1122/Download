xmlStringDecodeEntities(xmlParserCtxtPtr ctxt, const xmlChar *str, int what,
		        xmlChar end, xmlChar  end2, xmlChar end3) {
 if ((ctxt == NULL) || (str == NULL)) return(NULL);
 return(xmlStringLenDecodeEntities(ctxt, str, xmlStrlen(str), what,
           end, end2, end3));
}