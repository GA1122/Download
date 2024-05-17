void WebContentsImpl::SetPageScale(float page_scale_factor) {
  Send(new ViewMsg_SetPageScale(GetRenderViewHost()->GetRoutingID(),
                                page_scale_factor));
}
