WebContentsImpl* WebContentsImpl::GetOutermostWebContents() {
  WebContentsImpl* root = this;
  while (root->GetOuterWebContents())
    root = root->GetOuterWebContents();
  return root;
}
