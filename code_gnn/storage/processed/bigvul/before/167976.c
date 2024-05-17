void LocalFrame::SetIsAdSubframeIfNecessary() {
  DCHECK(ad_tracker_);
  Frame* parent = Tree().Parent();
  if (!parent)
    return;

  bool parent_is_ad =
      parent->IsLocalFrame() && ToLocalFrame(parent)->IsAdSubframe();

  if (parent_is_ad || ad_tracker_->IsAdScriptInStack())
    SetIsAdSubframe();
}
