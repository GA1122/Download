parse_list(xmlChar *str) {
    xmlChar **buffer;
    xmlChar **out = NULL;
    int buffer_size = 0;
    int len;

    if(str == NULL) {
	return(NULL);
    }

    len = xmlStrlen(str);
    if((str[0] == '\'') && (str[len - 1] == '\'')) {
	str[len - 1] = '\0';
	str++;
    }
     
    buffer_size = 1000;
    buffer = (xmlChar **) xmlMalloc(buffer_size * sizeof(xmlChar*));
    if (buffer == NULL) {
	perror("malloc failed");
	return(NULL);
    }
    out = buffer;

    while(*str != '\0') {
	if (out - buffer > buffer_size - 10) {
	    int indx = out - buffer;

	    xxx_growBufferReentrant();
	    out = &buffer[indx];
	}
	(*out++) = str;
	while(*str != ',' && *str != '\0') ++str;
	if(*str == ',') *(str++) = '\0';
    }
    (*out) = NULL;
    return buffer;
}