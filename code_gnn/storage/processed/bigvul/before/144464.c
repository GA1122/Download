WebContentsImpl* WebContentsImpl::CreateWithOpener(
    const WebContents::CreateParams& params,
    FrameTreeNode* opener) {
  TRACE_EVENT0("browser", "WebContentsImpl::CreateWithOpener");
  WebContentsImpl* new_contents = new WebContentsImpl(params.browser_context);

  FrameTreeNode* new_root = new_contents->GetFrameTree()->root();

  if (!params.opener_suppressed && opener) {
    new_root->SetOpener(opener);
    new_contents->created_with_opener_ = true;
  }

  if (opener) {
    blink::WebSandboxFlags opener_flags = opener->effective_sandbox_flags();
    const blink::WebSandboxFlags inherit_flag =
        blink::WebSandboxFlags::PropagatesToAuxiliaryBrowsingContexts;
    if ((opener_flags & inherit_flag) == inherit_flag) {
      new_root->SetPendingSandboxFlags(opener_flags);
      new_root->CommitPendingSandboxFlags();
    }
  }

  if (params.created_with_opener)
    new_contents->created_with_opener_ = true;

  if (params.guest_delegate) {
    BrowserPluginGuest::Create(new_contents, params.guest_delegate);
    new_contents->is_subframe_ = true;
  }
  new_contents->Init(params);
  return new_contents;
}
