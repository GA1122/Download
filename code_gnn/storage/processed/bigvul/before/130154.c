bool RenderFrameHostImpl::CanCommitURL(const GURL& url) {

  return GetContentClient()->browser()->CanCommitURL(GetProcess(), url);
}
