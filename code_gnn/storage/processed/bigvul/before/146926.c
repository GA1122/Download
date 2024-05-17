WebString WebLocalFrameImpl::AssignedName() const {
  return GetFrame()->Tree().GetName();
}
