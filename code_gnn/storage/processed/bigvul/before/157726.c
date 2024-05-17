std::unique_ptr<WebContents> WebContentsImpl::Clone() {
  CreateParams create_params(GetBrowserContext(), GetSiteInstance());
  create_params.initial_size = GetContainerBounds().size();
  FrameTreeNode* opener = frame_tree_.root()->opener();
  RenderFrameHostImpl* opener_rfh = nullptr;
  if (opener)
    opener_rfh = opener->current_frame_host();
  std::unique_ptr<WebContentsImpl> tc =
      CreateWithOpener(create_params, opener_rfh);
  tc->GetController().CopyStateFrom(controller_, true);
  for (auto& observer : observers_)
    observer.DidCloneToNewWebContents(this, tc.get());
  return tc;
}
