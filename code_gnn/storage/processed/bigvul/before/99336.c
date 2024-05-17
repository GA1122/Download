void ResourceMessageFilter::OnSetCookie(const IPC::Message& message,
                                        const GURL& url,
                                        const GURL& first_party_for_cookies,
                                        const std::string& cookie) {
  ChromeURLRequestContext* context = GetRequestContextForURL(url);

  scoped_ptr<Blacklist::Match> match(
      GetPrivacyBlacklistMatchForURL(url, context));
  if (match.get() && (match->attributes() & Blacklist::kBlockCookies))
    return;

  SetCookieCompletion* callback =
      new SetCookieCompletion(id(), message.routing_id(), url, cookie, context);

  int policy = net::OK;
  if (context->cookie_policy()) {
    policy = context->cookie_policy()->CanSetCookie(
        url, first_party_for_cookies, cookie, callback);
    if (policy == net::ERR_IO_PENDING)
      return;
  }
  callback->Run(policy);
}
