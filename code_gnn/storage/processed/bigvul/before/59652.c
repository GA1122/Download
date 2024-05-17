xmlParserPrintFileContextInternal(xmlParserInputPtr input ,
		xmlGenericErrorFunc chanl, void *data ) {
    const xmlChar *cur, *base;
    unsigned int n, col;	 
    xmlChar  content[81];  
    xmlChar *ctnt;

    if (input == NULL) return;
    cur = input->cur;
    base = input->base;
     
    while ((cur > base) && ((*(cur) == '\n') || (*(cur) == '\r'))) {
	cur--;
    }
    n = 0;
     
    while ((n++ < (sizeof(content)-1)) && (cur > base) &&
   (*(cur) != '\n') && (*(cur) != '\r'))
        cur--;
    if ((*(cur) == '\n') || (*(cur) == '\r')) cur++;
     
    col = input->cur - cur;
     
    n = 0;
    ctnt = content;
     
    while ((*cur != 0) && (*(cur) != '\n') &&
   (*(cur) != '\r') && (n < sizeof(content)-1)) {
		*ctnt++ = *cur++;
	n++;
    }
    *ctnt = 0;
     
    chanl(data ,"%s\n", content);
     
    n = 0;
    ctnt = content;
     
    while ((n<col) && (n++ < sizeof(content)-2) && (*ctnt != 0)) {
	if (*(ctnt) != '\t')
	    *(ctnt) = ' ';
	ctnt++;
    }
    *ctnt++ = '^';
    *ctnt = 0;
    chanl(data ,"%s\n", content);
}
