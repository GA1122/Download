xsltInitGlobals(void)
{
    if (xsltExtMutex == NULL) {
        xsltExtMutex = xmlNewMutex();
    }
}
