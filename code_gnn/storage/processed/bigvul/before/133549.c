void WebContentsImpl::DidDisownOpener(RenderViewHost* rvh) {
  if (opener_) {
    RemoveDestructionObserver(opener_);
    opener_ = NULL;
  }

  GetRenderManager()->DidDisownOpener(rvh);
}
