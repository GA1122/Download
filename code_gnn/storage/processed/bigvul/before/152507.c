void RenderFrameImpl::RunScriptsAtDocumentElementAvailable() {
  GetContentClient()->renderer()->RunScriptsAtDocumentStart(this);
}
