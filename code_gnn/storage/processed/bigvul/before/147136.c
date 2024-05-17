bool DocumentLoader::ShouldClearWindowName(
    const LocalFrame& frame,
    SecurityOrigin* previous_security_origin,
    const Document& new_document) {
  if (!previous_security_origin)
    return false;
  if (!frame.IsMainFrame())
    return false;
  if (frame.Loader().Opener())
    return false;

  return !new_document.GetSecurityOrigin()->IsSameSchemeHostPort(
      previous_security_origin);
}
