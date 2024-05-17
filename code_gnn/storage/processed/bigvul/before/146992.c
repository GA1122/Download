WebDocumentLoader* WebLocalFrameImpl::GetDocumentLoader() const {
  DCHECK(GetFrame());
  return DocumentLoaderForDocLoader(GetFrame()->Loader().GetDocumentLoader());
}
