WebLocalFrameImpl::MaybeRenderFallbackContent(const WebURLError& error) const {
  DCHECK(GetFrame());

  if (!GetFrame()->Owner() || !GetFrame()->Owner()->CanRenderFallbackContent())
    return NoFallbackContent;

  if (!GetFrame()->Loader().GetProvisionalDocumentLoader())
    return NoLoadInProgress;

  GetFrame()->Loader().GetProvisionalDocumentLoader()->LoadFailed(error);
  return FallbackRendered;
}
