void LocalFrameClientImpl::DownloadURL(
    const ResourceRequest& request,
    DownloadCrossOriginRedirects cross_origin_redirect_behavior) {
  if (!web_frame_->Client())
    return;
  DCHECK(web_frame_->GetFrame()->GetDocument());
  mojom::blink::BlobURLTokenPtr blob_url_token;
  if (request.Url().ProtocolIs("blob") && BlobUtils::MojoBlobURLsEnabled()) {
    web_frame_->GetFrame()->GetDocument()->GetPublicURLManager().Resolve(
        request.Url(), MakeRequest(&blob_url_token));
  }
  web_frame_->Client()->DownloadURL(
      WrappedResourceRequest(request),
      static_cast<WebLocalFrameClient::CrossOriginRedirects>(
          cross_origin_redirect_behavior),
      blob_url_token.PassInterface().PassHandle());
}
