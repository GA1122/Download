XML_SetEncoding(XML_Parser parser, const XML_Char *encodingName)
{
  if (parser == NULL)
      return XML_STATUS_ERROR;
   
  if (parser->m_parsingStatus.parsing == XML_PARSING || parser->m_parsingStatus.parsing == XML_SUSPENDED)
    return XML_STATUS_ERROR;

   
  FREE(parser, (void *)parser->m_protocolEncodingName);

  if (encodingName == NULL)
     
    parser->m_protocolEncodingName = NULL;
  else {
     
    parser->m_protocolEncodingName = copyString(encodingName, &(parser->m_mem));
    if (!parser->m_protocolEncodingName)
      return XML_STATUS_ERROR;
  }
  return XML_STATUS_OK;
}
