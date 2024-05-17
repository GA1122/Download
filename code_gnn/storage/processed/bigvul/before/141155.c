HTMLFrameOwnerElement* Document::LocalOwner() const {
  if (!GetFrame())
    return nullptr;
  return GetFrame()->DeprecatedLocalOwner();
}
