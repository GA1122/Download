xsltUTF8Charcmp(xmlChar *utf1, xmlChar *utf2) {

    if (utf1 == NULL ) {
        if (utf2 == NULL)
            return 0;
        return -1;
    }
    return xmlStrncmp(utf1, utf2, xsltUTF8Size(utf1));
}
