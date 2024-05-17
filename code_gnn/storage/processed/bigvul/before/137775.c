void HeadlessWebContentsImpl::RenderViewReady() {
  DCHECK(web_contents()->GetMainFrame()->IsRenderFrameLive());

  for (auto& observer : observers_)
    observer.DevToolsTargetReady();
}
