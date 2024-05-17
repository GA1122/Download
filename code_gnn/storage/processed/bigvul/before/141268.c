scoped_refptr<const SecurityOrigin> Document::TopFrameOrigin() const {
  if (!GetFrame())
    return scoped_refptr<const SecurityOrigin>();

  return GetFrame()->Tree().Top().GetSecurityContext()->GetSecurityOrigin();
}
