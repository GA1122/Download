WebLocalFrameImpl* WebLocalFrameImpl::LocalRoot() {
  WebLocalFrameImpl* local_root = this;
  while (local_root->Parent() && local_root->Parent()->IsWebLocalFrame())
    local_root = ToWebLocalFrameImpl(local_root->Parent());
  return local_root;
}
