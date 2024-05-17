 void XMLHttpRequest::genericError()
// void XMLHttpRequest::handleDidFailGeneric()
  {
      clearResponse();
      clearRequest();
    m_error = true;
  
    changeState(DONE);
//     m_error = true;
  }