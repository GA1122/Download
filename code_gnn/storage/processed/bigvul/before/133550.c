void WebContentsImpl::DidFailProvisionalLoadWithError(
    RenderFrameHostImpl* render_frame_host,
    const FrameHostMsg_DidFailProvisionalLoadWithError_Params& params) {
  GURL validated_url(params.url);
  FOR_EACH_OBSERVER(
      WebContentsObserver,
      observers_,
      DidFailProvisionalLoad(params.frame_id,
                             params.frame_unique_name,
                             params.is_main_frame,
                             validated_url,
                             params.error_code,
                             params.error_description,
                             render_frame_host->render_view_host()));
}
