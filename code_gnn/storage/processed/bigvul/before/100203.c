ScopableCPRequest::~ScopableCPRequest() {
  pdata = NULL;
  data = NULL;
  free(const_cast<char*>(url));
  free(const_cast<char*>(method));
}
