void RenderViewImpl::OpenURL(WebFrame* frame,
                             const GURL& url,
                             const Referrer& referrer,
                             WebNavigationPolicy policy) {
  ViewHostMsg_OpenURL_Params params;
  params.url = url;
  params.referrer = referrer;
  params.disposition = NavigationPolicyToDisposition(policy);
  params.frame_id = frame->identifier();
  WebDataSource* ds = frame->provisionalDataSource();
  if (ds) {
    params.is_cross_site_redirect = ds->isClientRedirect();
  } else {
    params.is_cross_site_redirect = false;
  }

  Send(new ViewHostMsg_OpenURL(routing_id_, params));
}
