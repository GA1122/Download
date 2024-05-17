bool XSSAuditor::IsSafeToSendToAnotherThread() const {
  return document_url_.IsSafeToSendToAnotherThread() &&
         decoded_url_.IsSafeToSendToAnotherThread() &&
         decoded_http_body_.IsSafeToSendToAnotherThread() &&
         http_body_as_string_.IsSafeToSendToAnotherThread();
}
