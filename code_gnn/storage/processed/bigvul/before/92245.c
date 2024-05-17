XML_GetBuffer(XML_Parser parser, int len)
{
  if (parser == NULL)
    return NULL;
  if (len < 0) {
    parser->m_errorCode = XML_ERROR_NO_MEMORY;
    return NULL;
  }
  switch (parser->m_parsingStatus.parsing) {
  case XML_SUSPENDED:
    parser->m_errorCode = XML_ERROR_SUSPENDED;
    return NULL;
  case XML_FINISHED:
    parser->m_errorCode = XML_ERROR_FINISHED;
    return NULL;
  default: ;
  }

  if (len > EXPAT_SAFE_PTR_DIFF(parser->m_bufferLim, parser->m_bufferEnd)) {
#ifdef XML_CONTEXT_BYTES
    int keep;
#endif   
     
    int neededSize = (int) ((unsigned)len +
                            (unsigned)EXPAT_SAFE_PTR_DIFF(parser->m_bufferEnd,
                                                          parser->m_bufferPtr));
    if (neededSize < 0) {
      parser->m_errorCode = XML_ERROR_NO_MEMORY;
      return NULL;
    }
#ifdef XML_CONTEXT_BYTES
    keep = (int)EXPAT_SAFE_PTR_DIFF(parser->m_bufferPtr, parser->m_buffer);
    if (keep > XML_CONTEXT_BYTES)
      keep = XML_CONTEXT_BYTES;
    neededSize += keep;
#endif   
    if (neededSize <= EXPAT_SAFE_PTR_DIFF(parser->m_bufferLim, parser->m_buffer)) {
#ifdef XML_CONTEXT_BYTES
      if (keep < EXPAT_SAFE_PTR_DIFF(parser->m_bufferPtr, parser->m_buffer)) {
          int offset = (int)EXPAT_SAFE_PTR_DIFF(parser->m_bufferPtr, parser->m_buffer) - keep;
         
        memmove(parser->m_buffer, &parser->m_buffer[offset], parser->m_bufferEnd - parser->m_bufferPtr + keep);
        parser->m_bufferEnd -= offset;
        parser->m_bufferPtr -= offset;
      }
#else
      if (parser->m_buffer && parser->m_bufferPtr) {
        memmove(parser->m_buffer, parser->m_bufferPtr,
                EXPAT_SAFE_PTR_DIFF(parser->m_bufferEnd, parser->m_bufferPtr));
        parser->m_bufferEnd = parser->m_buffer +
            EXPAT_SAFE_PTR_DIFF(parser->m_bufferEnd, parser->m_bufferPtr);
        parser->m_bufferPtr = parser->m_buffer;
      }
#endif   
    }
    else {
      char *newBuf;
      int bufferSize = (int)EXPAT_SAFE_PTR_DIFF(parser->m_bufferLim, parser->m_bufferPtr);
      if (bufferSize == 0)
        bufferSize = INIT_BUFFER_SIZE;
      do {
         
        bufferSize = (int) (2U * (unsigned) bufferSize);
      } while (bufferSize < neededSize && bufferSize > 0);
      if (bufferSize <= 0) {
        parser->m_errorCode = XML_ERROR_NO_MEMORY;
        return NULL;
      }
      newBuf = (char *)MALLOC(parser, bufferSize);
      if (newBuf == 0) {
        parser->m_errorCode = XML_ERROR_NO_MEMORY;
        return NULL;
      }
      parser->m_bufferLim = newBuf + bufferSize;
#ifdef XML_CONTEXT_BYTES
      if (parser->m_bufferPtr) {
        int keep = (int)EXPAT_SAFE_PTR_DIFF(parser->m_bufferPtr, parser->m_buffer);
        if (keep > XML_CONTEXT_BYTES)
          keep = XML_CONTEXT_BYTES;
        memcpy(newBuf, &parser->m_bufferPtr[-keep],
               EXPAT_SAFE_PTR_DIFF(parser->m_bufferEnd, parser->m_bufferPtr) + keep);
        FREE(parser, parser->m_buffer);
        parser->m_buffer = newBuf;
        parser->m_bufferEnd = parser->m_buffer +
            EXPAT_SAFE_PTR_DIFF(parser->m_bufferEnd, parser->m_bufferPtr) + keep;
        parser->m_bufferPtr = parser->m_buffer + keep;
      }
      else {
         
        parser->m_bufferEnd = newBuf;
        parser->m_bufferPtr = parser->m_buffer = newBuf;
      }
#else
      if (parser->m_bufferPtr) {
        memcpy(newBuf, parser->m_bufferPtr,
               EXPAT_SAFE_PTR_DIFF(parser->m_bufferEnd, parser->m_bufferPtr));
        FREE(parser, parser->m_buffer);
        parser->m_bufferEnd = newBuf +
            EXPAT_SAFE_PTR_DIFF(parser->m_bufferEnd, parser->m_bufferPtr);
      }
      else {
         
        parser->m_bufferEnd = newBuf;
      }
      parser->m_bufferPtr = parser->m_buffer = newBuf;
#endif   
    }
    parser->m_eventPtr = parser->m_eventEndPtr = NULL;
    parser->m_positionPtr = NULL;
  }
  return parser->m_bufferEnd;
}
