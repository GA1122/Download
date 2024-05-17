void RenderFrameImpl::RunScriptsAtDocumentIdle() {
  GetContentClient()->renderer()->RunScriptsAtDocumentIdle(this);
}
