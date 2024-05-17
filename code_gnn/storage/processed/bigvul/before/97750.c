xmlXPathNormalizeFunction(xmlXPathParserContextPtr ctxt, int nargs) {
  xmlXPathObjectPtr obj = NULL;
  xmlChar *source = NULL;
  xmlBufferPtr target;
  xmlChar blank;

  if (ctxt == NULL) return;
  if (nargs == 0) {
     
      valuePush(ctxt,
	  xmlXPathCacheWrapString(ctxt->context,
	    xmlXPathCastNodeToString(ctxt->context->node)));
    nargs = 1;
  }

  CHECK_ARITY(1);
  CAST_TO_STRING;
  CHECK_TYPE(XPATH_STRING);
  obj = valuePop(ctxt);
  source = obj->stringval;

  target = xmlBufferCreate();
  if (target && source) {

     
    while (IS_BLANK_CH(*source))
      source++;

     
    blank = 0;
    while (*source) {
      if (IS_BLANK_CH(*source)) {
	blank = 0x20;
      } else {
	if (blank) {
	  xmlBufferAdd(target, &blank, 1);
	  blank = 0;
	}
	xmlBufferAdd(target, source, 1);
      }
      source++;
    }
    valuePush(ctxt, xmlXPathCacheNewString(ctxt->context,
	xmlBufferContent(target)));
    xmlBufferFree(target);
  }
  xmlXPathReleaseObject(ctxt->context, obj);
}
