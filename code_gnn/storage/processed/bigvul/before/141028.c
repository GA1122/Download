void Document::DidUpdateSecurityOrigin() {
  if (!frame_)
    return;
  frame_->GetScriptController().UpdateSecurityOrigin(GetSecurityOrigin());
}
