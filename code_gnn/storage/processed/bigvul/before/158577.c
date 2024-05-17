void WebLocalFrameImpl::SetIsAdSubframe() {
  DCHECK(GetFrame());
  GetFrame()->SetIsAdSubframe();
}
