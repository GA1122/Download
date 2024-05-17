bool WebLocalFrameImpl::IsAdSubframe() const {
  DCHECK(GetFrame());
  return GetFrame()->IsAdSubframe();
}
