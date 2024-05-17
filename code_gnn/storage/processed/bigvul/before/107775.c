bool FrameLoader::ShouldReuseDefaultView(const KURL& url,
                                         const ContentSecurityPolicy* csp) {
  if (!state_machine_.IsDisplayingInitialEmptyDocument())
    return false;

  if (csp && (csp->GetSandboxMask() & kSandboxOrigin)) {
    return false;
  }

  scoped_refptr<const SecurityOrigin> origin = SecurityOrigin::Create(url);
  return frame_->GetDocument()->GetSecurityOrigin()->CanAccess(origin.get());
}
