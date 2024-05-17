xmlAddDefAttrs(xmlParserCtxtPtr ctxt,
 const xmlChar *fullname,
 const xmlChar *fullattr,
 const xmlChar *value) {
    xmlDefAttrsPtr defaults;
 int len;
 const xmlChar *name;
 const xmlChar *prefix;

  
 if (ctxt->attsSpecial != NULL) {
 if (xmlHashLookup2(ctxt->attsSpecial, fullname, fullattr) != NULL)
 return;
 }

 if (ctxt->attsDefault == NULL) {
        ctxt->attsDefault = xmlHashCreateDict(10, ctxt->dict);
 if (ctxt->attsDefault == NULL)
 goto mem_error;
 }

  
    name = xmlSplitQName3(fullname, &len);
 if (name == NULL) {
        name = xmlDictLookup(ctxt->dict, fullname, -1);
	prefix = NULL;
 } else {
        name = xmlDictLookup(ctxt->dict, name, -1);
	prefix = xmlDictLookup(ctxt->dict, fullname, len);
 }

  
    defaults = xmlHashLookup2(ctxt->attsDefault, name, prefix);
 if (defaults == NULL) {
        defaults = (xmlDefAttrsPtr) xmlMalloc(sizeof(xmlDefAttrs) +
 (4 * 5) * sizeof(const xmlChar *));
 if (defaults == NULL)
 goto mem_error;
	defaults->nbAttrs = 0;
	defaults->maxAttrs = 4;
 if (xmlHashUpdateEntry2(ctxt->attsDefault, name, prefix,
	                        defaults, NULL) < 0) {
	    xmlFree(defaults);
 goto mem_error;
 }
 } else if (defaults->nbAttrs >= defaults->maxAttrs) {
        xmlDefAttrsPtr temp;

        temp = (xmlDefAttrsPtr) xmlRealloc(defaults, sizeof(xmlDefAttrs) +
 (2 * defaults->maxAttrs * 5) * sizeof(const xmlChar *));
 if (temp == NULL)
 goto mem_error;
	defaults = temp;
	defaults->maxAttrs *= 2;
 if (xmlHashUpdateEntry2(ctxt->attsDefault, name, prefix,
	                        defaults, NULL) < 0) {
	    xmlFree(defaults);
 goto mem_error;
 }
 }

  
    name = xmlSplitQName3(fullattr, &len);
 if (name == NULL) {
        name = xmlDictLookup(ctxt->dict, fullattr, -1);
	prefix = NULL;
 } else {
        name = xmlDictLookup(ctxt->dict, name, -1);
	prefix = xmlDictLookup(ctxt->dict, fullattr, len);
 }

    defaults->values[5 * defaults->nbAttrs] = name;
    defaults->values[5 * defaults->nbAttrs + 1] = prefix;
  
    len = xmlStrlen(value);
    value = xmlDictLookup(ctxt->dict, value, len);
    defaults->values[5 * defaults->nbAttrs + 2] = value;
    defaults->values[5 * defaults->nbAttrs + 3] = value + len;
 if (ctxt->external)
        defaults->values[5 * defaults->nbAttrs + 4] = BAD_CAST "external";
 else
        defaults->values[5 * defaults->nbAttrs + 4] = NULL;
    defaults->nbAttrs++;

 return;

mem_error:
    xmlErrMemory(ctxt, NULL);
 return;
}