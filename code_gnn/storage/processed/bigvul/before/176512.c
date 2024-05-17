xmlCreateURI(void) {
    xmlURIPtr ret;

    ret = (xmlURIPtr) xmlMalloc(sizeof(xmlURI));
 if (ret == NULL) {
        xmlURIErrMemory("creating URI structure\n");
 return(NULL);
 }
    memset(ret, 0, sizeof(xmlURI));
 return(ret);
}
