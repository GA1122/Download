build_model(XML_Parser parser) {
  DTD *const dtd = parser->m_dtd;  
  XML_Content *ret;
  XML_Content *cpos;
  XML_Char *str;
  int allocsize = (dtd->scaffCount * sizeof(XML_Content)
                   + (dtd->contentStringLen * sizeof(XML_Char)));

  ret = (XML_Content *)MALLOC(parser, allocsize);
  if (! ret)
    return NULL;

  str = (XML_Char *)(&ret[dtd->scaffCount]);
  cpos = &ret[1];

  build_node(parser, 0, ret, &cpos, &str);
  return ret;
}