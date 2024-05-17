void WebGLRenderingContextBase::ForciblyLoseOldestContext(
    const String& reason) {
  WebGLRenderingContextBase* candidate = OldestContext();
  if (!candidate)
    return;

  candidate->PrintWarningToConsole(reason);
  probe::didFireWebGLWarning(candidate->canvas());

  candidate->ForceLostContext(WebGLRenderingContextBase::kSyntheticLostContext,
                              WebGLRenderingContextBase::kWhenAvailable);
}
