void WebContentsImpl::OnDidFinishLoad(RenderFrameHostImpl* source,
                                      const GURL& url) {
  GURL validated_url(url);
  source->GetProcess()->FilterURL(false, &validated_url);

  for (auto& observer : observers_)
    observer.DidFinishLoad(source, validated_url);
}
