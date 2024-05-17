void RenderMessageFilter::SendGetRawCookiesResponse(
    IPC::Message* reply_msg,
    const net::CookieList& cookie_list) {
  std::vector<webkit_glue::WebCookie> cookies;
  for (size_t i = 0; i < cookie_list.size(); ++i)
    cookies.push_back(webkit_glue::WebCookie(cookie_list[i]));
  ViewHostMsg_GetRawCookies::WriteReplyParams(reply_msg, cookies);
  Send(reply_msg);
}
