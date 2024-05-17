startParsing(XML_Parser parser) {
   
  if (parser->m_hash_secret_salt == 0)
    parser->m_hash_secret_salt = generate_hash_secret_salt(parser);
  if (parser->m_ns) {
     
    return setContext(parser, implicitContext);
  }
  return XML_TRUE;
}
