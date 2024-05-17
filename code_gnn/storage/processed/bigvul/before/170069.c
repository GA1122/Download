xsltFreeStylesheet(xsltStylesheetPtr style)
{
    if (style == NULL)
        return;

#ifdef XSLT_REFACTORED
     
    if ((style->principal == style) && (style->doc))
	xsltCleanupStylesheetTree(style->doc,
	    xmlDocGetRootElement(style->doc));
#ifdef XSLT_REFACTORED_XSLT_NSCOMP
     
    if ((style->doc != NULL) &&
	XSLT_HAS_INTERNAL_NSMAP(style))
    {
	xsltRestoreDocumentNamespaces(XSLT_GET_INTERNAL_NSMAP(style),
	    style->doc);
    }
#endif  
#else
     
    if ((style->parent == NULL) && (style->doc))
	xsltCleanupStylesheetTree(style->doc,
	    xmlDocGetRootElement(style->doc));
#endif  

    xsltFreeKeys(style);
    xsltFreeExts(style);
    xsltFreeTemplateHashes(style);
    xsltFreeDecimalFormatList(style);
    xsltFreeTemplateList(style->templates);
    xsltFreeAttributeSetsHashes(style);
    xsltFreeNamespaceAliasHashes(style);
    xsltFreeStylePreComps(style);
     
    xsltFreeStyleDocuments(style);
     
    xsltShutdownExts(style);

    if (style->variables != NULL)
        xsltFreeStackElemList(style->variables);
    if (style->cdataSection != NULL)
        xmlHashFree(style->cdataSection, NULL);
    if (style->stripSpaces != NULL)
        xmlHashFree(style->stripSpaces, NULL);
    if (style->nsHash != NULL)
        xmlHashFree(style->nsHash, NULL);
    if (style->exclPrefixTab != NULL)
        xmlFree(style->exclPrefixTab);
    if (style->method != NULL)
        xmlFree(style->method);
    if (style->methodURI != NULL)
        xmlFree(style->methodURI);
    if (style->version != NULL)
        xmlFree(style->version);
    if (style->encoding != NULL)
        xmlFree(style->encoding);
    if (style->doctypePublic != NULL)
        xmlFree(style->doctypePublic);
    if (style->doctypeSystem != NULL)
        xmlFree(style->doctypeSystem);
    if (style->mediaType != NULL)
        xmlFree(style->mediaType);
    if (style->attVTs)
        xsltFreeAVTList(style->attVTs);
    if (style->imports != NULL)
        xsltFreeStylesheetList(style->imports);

#ifdef XSLT_REFACTORED
     
    if (style->principal == style) {
	if (style->principalData) {
	    xsltFreePrincipalStylesheetData(style->principalData);
	    style->principalData = NULL;
	}
    }
#endif
     
    if (style->doc != NULL) {
        xmlFreeDoc(style->doc);
    }

#ifdef WITH_XSLT_DEBUG
    xsltGenericDebug(xsltGenericDebugContext,
                     "freeing dictionary from stylesheet\n");
#endif
    xmlDictFree(style->dict);

    memset(style, -1, sizeof(xsltStylesheet));
    xmlFree(style);
}
