WebContentsImpl* WebContentsImpl::CreateWithOpener(
    const WebContents::CreateParams& params,
    RenderFrameHostImpl* opener_rfh) {
  TRACE_EVENT0("browser", "WebContentsImpl::CreateWithOpener");
  FrameTreeNode* opener = nullptr;
  if (opener_rfh)
    opener = opener_rfh->frame_tree_node();
  WebContentsImpl* new_contents = new WebContentsImpl(params.browser_context);
  new_contents->SetOpenerForNewContents(opener, params.opener_suppressed);

  FrameTreeNode* new_root = new_contents->GetFrameTree()->root();
  if (opener) {
    blink::WebSandboxFlags opener_flags = opener_rfh->active_sandbox_flags();
    const blink::WebSandboxFlags inherit_flag =
        blink::WebSandboxFlags::kPropagatesToAuxiliaryBrowsingContexts;
    if ((opener_flags & inherit_flag) == inherit_flag) {
      new_root->SetPendingFramePolicy({opener_flags, {}});
    }
  }

  blink::FramePolicy frame_policy(new_root->pending_frame_policy());
  frame_policy.sandbox_flags |= params.starting_sandbox_flags;
  new_root->SetPendingFramePolicy(frame_policy);
  new_root->CommitPendingFramePolicy();

  if (params.created_with_opener)
    new_contents->created_with_opener_ = true;

  if (params.guest_delegate) {
    BrowserPluginGuest::Create(new_contents, params.guest_delegate);
    new_contents->is_subframe_ = true;
  }

  new_contents->Init(params);
  return new_contents;
}
