std::string UrlmonUrlRequest::me() const {
  return base::StringPrintf(" id: %i Obj: %X ", id(), this);
}
