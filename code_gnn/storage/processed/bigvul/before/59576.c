entityDeclDebug(void *ctx ATTRIBUTE_UNUSED, const xmlChar *name, int type,
          const xmlChar *publicId, const xmlChar *systemId, xmlChar *content)
{
const xmlChar *nullstr = BAD_CAST "(null)";
     
    if (publicId == NULL)
        publicId = nullstr;
    if (systemId == NULL)
        systemId = nullstr;
    if (content == NULL)
        content = (xmlChar *)nullstr;
    callbacks++;
    if (quiet)
	return;
    fprintf(SAXdebug, "SAX.entityDecl(%s, %d, %s, %s, %s)\n",
            name, type, publicId, systemId, content);
}