UrlFetcher::Core::Core(const GURL& url, Method method)
    : url_(url),
      method_(method),
      delegate_message_loop_(base::MessageLoopProxy::current()),
      buffer_(new net::IOBuffer(kBufferSize)) {
  CHECK(url_.is_valid());
}
