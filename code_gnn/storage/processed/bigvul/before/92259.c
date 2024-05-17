XML_Parse(XML_Parser parser, const char *s, int len, int isFinal)
{
  if ((parser == NULL) || (len < 0) || ((s == NULL) && (len != 0))) {
    if (parser != NULL)
      parser->m_errorCode = XML_ERROR_INVALID_ARGUMENT;
    return XML_STATUS_ERROR;
  }
  switch (parser->m_parsingStatus.parsing) {
  case XML_SUSPENDED:
    parser->m_errorCode = XML_ERROR_SUSPENDED;
    return XML_STATUS_ERROR;
  case XML_FINISHED:
    parser->m_errorCode = XML_ERROR_FINISHED;
    return XML_STATUS_ERROR;
  case XML_INITIALIZED:
    if (parser->m_parentParser == NULL && !startParsing(parser)) {
      parser->m_errorCode = XML_ERROR_NO_MEMORY;
      return XML_STATUS_ERROR;
    }
     
  default:
    parser->m_parsingStatus.parsing = XML_PARSING;
  }

  if (len == 0) {
    parser->m_parsingStatus.finalBuffer = (XML_Bool)isFinal;
    if (!isFinal)
      return XML_STATUS_OK;
    parser->m_positionPtr = parser->m_bufferPtr;
    parser->m_parseEndPtr = parser->m_bufferEnd;

     
    parser->m_errorCode = parser->m_processor(parser, parser->m_bufferPtr, parser->m_parseEndPtr, &parser->m_bufferPtr);

    if (parser->m_errorCode == XML_ERROR_NONE) {
      switch (parser->m_parsingStatus.parsing) {
      case XML_SUSPENDED:
         
        XmlUpdatePosition(parser->m_encoding, parser->m_positionPtr, parser->m_bufferPtr, &parser->m_position);
        parser->m_positionPtr = parser->m_bufferPtr;
        return XML_STATUS_SUSPENDED;
         
      case XML_INITIALIZED:
      case XML_PARSING:
        parser->m_parsingStatus.parsing = XML_FINISHED;
         
      default:
        return XML_STATUS_OK;
      }
    }
    parser->m_eventEndPtr = parser->m_eventPtr;
    parser->m_processor = errorProcessor;
    return XML_STATUS_ERROR;
  }
#ifndef XML_CONTEXT_BYTES
  else if (parser->m_bufferPtr == parser->m_bufferEnd) {
    const char *end;
    int nLeftOver;
    enum XML_Status result;
     
    if (len > ((XML_Size)-1) / 2 - parser->m_parseEndByteIndex) {
       parser->m_errorCode = XML_ERROR_NO_MEMORY;
       parser->m_eventPtr = parser->m_eventEndPtr = NULL;
       parser->m_processor = errorProcessor;
       return XML_STATUS_ERROR;
    }
    parser->m_parseEndByteIndex += len;
    parser->m_positionPtr = s;
    parser->m_parsingStatus.finalBuffer = (XML_Bool)isFinal;

    parser->m_errorCode = parser->m_processor(parser, s, parser->m_parseEndPtr = s + len, &end);

    if (parser->m_errorCode != XML_ERROR_NONE) {
      parser->m_eventEndPtr = parser->m_eventPtr;
      parser->m_processor = errorProcessor;
      return XML_STATUS_ERROR;
    }
    else {
      switch (parser->m_parsingStatus.parsing) {
      case XML_SUSPENDED:
        result = XML_STATUS_SUSPENDED;
        break;
      case XML_INITIALIZED:
      case XML_PARSING:
        if (isFinal) {
          parser->m_parsingStatus.parsing = XML_FINISHED;
          return XML_STATUS_OK;
        }
       
      default:
        result = XML_STATUS_OK;
      }
    }

    XmlUpdatePosition(parser->m_encoding, parser->m_positionPtr, end, &parser->m_position);
    nLeftOver = s + len - end;
    if (nLeftOver) {
      if (parser->m_buffer == NULL || nLeftOver > parser->m_bufferLim - parser->m_buffer) {
         
        char *temp = NULL;
        const int bytesToAllocate = (int)((unsigned)len * 2U);
        if (bytesToAllocate > 0) {
          temp = (char *)REALLOC(parser, parser->m_buffer, bytesToAllocate);
        }
        if (temp == NULL) {
          parser->m_errorCode = XML_ERROR_NO_MEMORY;
          parser->m_eventPtr = parser->m_eventEndPtr = NULL;
          parser->m_processor = errorProcessor;
          return XML_STATUS_ERROR;
        }
        parser->m_buffer = temp;
        parser->m_bufferLim = parser->m_buffer + bytesToAllocate;
      }
      memcpy(parser->m_buffer, end, nLeftOver);
    }
    parser->m_bufferPtr = parser->m_buffer;
    parser->m_bufferEnd = parser->m_buffer + nLeftOver;
    parser->m_positionPtr = parser->m_bufferPtr;
    parser->m_parseEndPtr = parser->m_bufferEnd;
    parser->m_eventPtr = parser->m_bufferPtr;
    parser->m_eventEndPtr = parser->m_bufferPtr;
    return result;
  }
#endif   
  else {
    void *buff = XML_GetBuffer(parser, len);
    if (buff == NULL)
      return XML_STATUS_ERROR;
    else {
      memcpy(buff, s, len);
      return XML_ParseBuffer(parser, len, isFinal);
    }
  }
}