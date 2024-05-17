std::string SessionManager::GetAddress() {
  std::string hostname = net::GetHostName();
#if defined(OS_WIN)
  if (hostname.length()) {
    struct hostent* host_entry = gethostbyname(hostname.c_str());
    if (host_entry)
      hostname = host_entry->h_name;
  }
#endif
  if (hostname.empty()) {
    hostname = "localhost";
  }
  return hostname + ":" + port_ + url_base_;
}
