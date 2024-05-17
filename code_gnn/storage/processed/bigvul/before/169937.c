xsltUTF8Size(xmlChar *utf) {
    xmlChar mask;
    int len;

    if (utf == NULL)
        return -1;
    if (*utf < 0x80)
        return 1;
     
    if (!(*utf & 0x40))
        return -1;
     
    len = 2;
    for (mask=0x20; mask != 0; mask>>=1) {
        if (!(*utf & mask))
            return len;
        len++;
    }
    return -1;
}
