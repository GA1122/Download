void RenderViewImpl::OpenURL(WebFrame* frame,
                             const GURL& url,
                             const Referrer& referrer,
                             WebNavigationPolicy policy) {
  Send(new ViewHostMsg_OpenURL(
      routing_id_,
      url,
      referrer,
      NavigationPolicyToDisposition(policy),
      frame->identifier()));
}
