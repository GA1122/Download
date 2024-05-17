WebDocumentLoader* WebLocalFrameImpl::GetProvisionalDocumentLoader() const {
  DCHECK(GetFrame());
  return DocumentLoaderForDocLoader(
      GetFrame()->Loader().GetProvisionalDocumentLoader());
}
