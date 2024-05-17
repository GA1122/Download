HTMLFrameOwnerElement* Document::LocalOwner() const {
  if (!GetFrame())
    return 0;
  return GetFrame()->DeprecatedLocalOwner();
}
