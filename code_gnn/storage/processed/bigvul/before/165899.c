void RenderFrameImpl::DownloadURL(
    const blink::WebURLRequest& request,
    CrossOriginRedirects cross_origin_redirect_behavior,
    mojo::ScopedMessagePipeHandle blob_url_token) {
  if (ShouldThrottleDownload())
    return;

  FrameHostMsg_DownloadUrl_Params params;
  params.render_view_id = render_view_->GetRoutingID();
  params.render_frame_id = GetRoutingID();
  params.url = request.Url();
  params.referrer = RenderViewImpl::GetReferrerFromRequest(frame_, request);
  params.initiator_origin = request.RequestorOrigin();
  if (request.GetSuggestedFilename().has_value())
    params.suggested_name = request.GetSuggestedFilename()->Utf16();
  params.follow_cross_origin_redirects =
      (cross_origin_redirect_behavior == CrossOriginRedirects::kFollow);
  params.blob_url_token = blob_url_token.release();

  Send(new FrameHostMsg_DownloadUrl(params));
}
