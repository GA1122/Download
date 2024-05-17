void RenderFrameImpl::GetCanonicalUrlForSharing(
    GetCanonicalUrlForSharingCallback callback) {
  WebURL canonical_url = GetWebFrame()->GetDocument().CanonicalUrlForSharing();
  std::move(callback).Run(canonical_url.IsNull()
                              ? base::nullopt
                              : base::make_optional(GURL(canonical_url)));
}
