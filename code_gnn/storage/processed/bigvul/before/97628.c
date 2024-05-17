xmlXPathCompile(const xmlChar *str) {
    return(xmlXPathCtxtCompile(NULL, str));
}
