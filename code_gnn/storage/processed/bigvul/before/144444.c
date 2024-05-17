WebContents* WebContentsImpl::Clone() {
  CreateParams create_params(GetBrowserContext(), GetSiteInstance());
  create_params.initial_size = GetContainerBounds().size();
  WebContentsImpl* tc =
      CreateWithOpener(create_params, frame_tree_.root()->opener());
  tc->GetController().CopyStateFrom(controller_);
  FOR_EACH_OBSERVER(WebContentsObserver,
                    observers_,
                    DidCloneToNewWebContents(this, tc));
  return tc;
}
