void ResourceMessageFilter::OnGetRawCookies(
    const GURL& url,
    const GURL& first_party_for_cookies,
    IPC::Message* reply_msg) {

  ChromeURLRequestContext* context = GetRequestContextForURL(url);

  if (!ChildProcessSecurityPolicy::GetInstance()->CanReadRawCookies(id()) ||
      context->IsExternal()) {
    ViewHostMsg_GetRawCookies::WriteReplyParams(
        reply_msg,
        std::vector<webkit_glue::WebCookie>());
    Send(reply_msg);
    return;
  }

  GetRawCookiesCompletion* callback =
      new GetRawCookiesCompletion(url, reply_msg, this, context);

  int policy = net::OK;
  if (context->cookie_policy()) {
    policy = context->cookie_policy()->CanGetCookies(
       url, first_party_for_cookies, callback);
    if (policy == net::ERR_IO_PENDING) {
      Send(new ViewMsg_SignalCookiePromptEvent());
      return;
    }
  }
  callback->Run(policy);
}
