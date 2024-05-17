resolveEntityDebug(void *ctx ATTRIBUTE_UNUSED, const xmlChar *publicId, const xmlChar *systemId)
{
    callbacks++;
    if (quiet)
	return(NULL);
     


    fprintf(SAXdebug, "SAX.resolveEntity(");
    if (publicId != NULL)
	fprintf(SAXdebug, "%s", (char *)publicId);
    else
	fprintf(SAXdebug, " ");
    if (systemId != NULL)
	fprintf(SAXdebug, ", %s)\n", (char *)systemId);
    else
	fprintf(SAXdebug, ", )\n");
 
    return(NULL);
}
