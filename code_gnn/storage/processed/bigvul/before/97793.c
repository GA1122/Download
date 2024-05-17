xmlXPathSubstringBeforeFunction(xmlXPathParserContextPtr ctxt, int nargs) {
  xmlXPathObjectPtr str;
  xmlXPathObjectPtr find;
  xmlBufferPtr target;
  const xmlChar *point;
  int offset;

  CHECK_ARITY(2);
  CAST_TO_STRING;
  find = valuePop(ctxt);
  CAST_TO_STRING;
  str = valuePop(ctxt);

  target = xmlBufferCreate();
  if (target) {
    point = xmlStrstr(str->stringval, find->stringval);
    if (point) {
      offset = (int)(point - str->stringval);
      xmlBufferAdd(target, str->stringval, offset);
    }
    valuePush(ctxt, xmlXPathCacheNewString(ctxt->context,
	xmlBufferContent(target)));
    xmlBufferFree(target);
  }
  xmlXPathReleaseObject(ctxt->context, str);
  xmlXPathReleaseObject(ctxt->context, find);
}
