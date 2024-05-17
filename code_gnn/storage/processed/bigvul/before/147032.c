void WebLocalFrameImpl::MixedContentFound(
    const WebURL& main_resource_url,
    const WebURL& mixed_content_url,
    WebURLRequest::RequestContext request_context,
    bool was_allowed,
    bool had_redirect,
    const WebSourceLocation& source_location) {
  DCHECK(GetFrame());
  std::unique_ptr<SourceLocation> source;
  if (!source_location.url.IsNull()) {
    source =
        SourceLocation::Create(source_location.url, source_location.line_number,
                               source_location.column_number, nullptr);
  }
  MixedContentChecker::MixedContentFound(
      GetFrame(), main_resource_url, mixed_content_url, request_context,
      was_allowed, had_redirect, std::move(source));
}
