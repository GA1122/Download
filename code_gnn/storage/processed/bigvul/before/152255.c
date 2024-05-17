void RenderFrameImpl::DidChangeName(const blink::WebString& name) {
  if (current_history_item_.IsNull()) {
    unique_name_helper_.UpdateName(name.Utf8());
  }
  GetFrameHost()->DidChangeName(name.Utf8(), unique_name_helper_.value());

  if (!committed_first_load_)
    name_changed_before_first_commit_ = true;
}
