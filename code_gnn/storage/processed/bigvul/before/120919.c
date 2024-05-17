bool SocketStream::Delegate::CanSetCookie(SocketStream* request,
                                          const GURL& url,
                                          const std::string& cookie_line,
                                          CookieOptions* options) {
  return true;
}
