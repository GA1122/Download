bool RenderFrameHostImpl::CanCommitURL(const GURL& url) {
  if (IsRendererDebugURL(url))
    return false;


  return GetContentClient()->browser()->CanCommitURL(GetProcess(), url);
}
