void NotifyResponseOnUI(int render_process_id,
                        int render_frame_host,
                        scoped_ptr<ResourceRequestDetails> details) {
  RenderFrameHostImpl* host =
      RenderFrameHostImpl::FromID(render_process_id, render_frame_host);
  WebContentsImpl* web_contents =
      static_cast<WebContentsImpl*>(WebContents::FromRenderFrameHost(host));
  if (!web_contents)
    return;
  web_contents->DidGetResourceResponseStart(*details.get());
}
