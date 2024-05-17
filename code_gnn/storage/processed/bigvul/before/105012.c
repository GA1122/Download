void HttpBridge::SetURL(const char* url, int port) {
  DCHECK_EQ(MessageLoop::current(), created_on_loop_);
  if (DCHECK_IS_ON()) {
    base::AutoLock lock(fetch_state_lock_);
    DCHECK(!fetch_state_.request_completed);
  }
  DCHECK(url_for_request_.is_empty())
      << "HttpBridge::SetURL called more than once?!";
  GURL temp(url);
  GURL::Replacements replacements;
  std::string port_str = base::IntToString(port);
  replacements.SetPort(port_str.c_str(),
                       url_parse::Component(0, port_str.length()));
  url_for_request_ = temp.ReplaceComponents(replacements);
}
