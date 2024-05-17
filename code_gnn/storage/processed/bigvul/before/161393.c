Response SecurityHandler::HandleCertificateError(int event_id,
                                                 const String& action) {
  if (cert_error_callbacks_.find(event_id) == cert_error_callbacks_.end()) {
    return Response::Error(
        String("Unknown event id: " + std::to_string(event_id)));
  }
  content::CertificateRequestResultType type =
      content::CERTIFICATE_REQUEST_RESULT_TYPE_CANCEL;
  Response response = Response::OK();
  if (action == Security::CertificateErrorActionEnum::Continue) {
    type = content::CERTIFICATE_REQUEST_RESULT_TYPE_CONTINUE;
  } else if (action == Security::CertificateErrorActionEnum::Cancel) {
    type = content::CERTIFICATE_REQUEST_RESULT_TYPE_CANCEL;
  } else {
    response =
        Response::Error(String("Unknown Certificate Error Action: " + action));
  }
  cert_error_callbacks_[event_id].Run(type);
  cert_error_callbacks_.erase(event_id);
  return response;
}
