 bool LocalFrame::ShouldReuseDefaultView(const KURL& url) const {
// bool LocalFrame::ShouldReuseDefaultView(
//     const KURL& url,
//     const ContentSecurityPolicy* csp) const {
    if (!Loader().StateMachine()->IsDisplayingInitialEmptyDocument())
      return false;
  
//    
//    
//    
//    
//    
//   if (csp &&
//       SecurityContext::IsSandboxed(kSandboxOrigin, csp->GetSandboxMask())) {
//     return false;
//   }
// 
    return GetDocument()->IsSecureTransitionTo(url);
  }