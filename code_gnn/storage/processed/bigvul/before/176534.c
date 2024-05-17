xmlSaveUriRealloc(xmlChar *ret, int *max) {
    xmlChar *temp;
 int tmp;

 if (*max > MAX_URI_LENGTH) {
        xmlURIErrMemory("reaching arbitrary MAX_URI_LENGTH limit\n");
 return(NULL);
 }
    tmp = *max * 2;
    temp = (xmlChar *) xmlRealloc(ret, (tmp + 1));
 if (temp == NULL) {
        xmlURIErrMemory("saving URI\n");
 return(NULL);
 }
 *max = tmp;
 return(temp);
}
