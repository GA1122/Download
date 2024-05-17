freeBindings(XML_Parser parser, BINDING *bindings) {
  while (bindings) {
    BINDING *b = bindings;

     
    if (parser->m_endNamespaceDeclHandler)
      parser->m_endNamespaceDeclHandler(parser->m_handlerArg, b->prefix->name);

    bindings = bindings->nextTagBinding;
    b->nextTagBinding = parser->m_freeBindingList;
    parser->m_freeBindingList = b;
    b->prefix->binding = b->prevPrefixBinding;
  }
}
