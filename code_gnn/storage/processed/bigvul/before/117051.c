BaseSessionService::InternalGetCommandsRequest::~InternalGetCommandsRequest() {
  STLDeleteElements(&commands);
}
