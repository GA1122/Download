void WebContentsImpl::SetPageScale(float page_scale_factor) {
  Send(new ViewMsg_SetPageScale(GetRoutingID(), page_scale_factor));
}
