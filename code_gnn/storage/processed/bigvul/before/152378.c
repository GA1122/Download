void RenderFrameImpl::HandleRendererDebugURL(const GURL& url) {
  DCHECK(IsRendererDebugURL(url));
  base::WeakPtr<RenderFrameImpl> weak_this = weak_factory_.GetWeakPtr();
  if (url.SchemeIs(url::kJavaScriptScheme)) {
    frame_->LoadJavaScriptURL(url);
  } else {
    HandleChromeDebugURL(url);
  }

  if (weak_this && frame_ && !frame_->IsLoading())
    Send(new FrameHostMsg_DidStopLoading(routing_id_));
}
