void WebContentsImpl::RunFileChooser(RenderFrameHost* render_frame_host,
                                     const FileChooserParams& params) {
  if (delegate_)
    delegate_->RunFileChooser(render_frame_host, params);
}
