void RenderFrameHostImpl::OnContextMenu(const ContextMenuParams& params) {
  ContextMenuParams validated_params(params);
  RenderProcessHost* process = GetProcess();

  process->FilterURL(true, &validated_params.link_url);
  process->FilterURL(true, &validated_params.src_url);
  process->FilterURL(false, &validated_params.page_url);
  process->FilterURL(true, &validated_params.frame_url);

  delegate_->ShowContextMenu(this, validated_params);
}
