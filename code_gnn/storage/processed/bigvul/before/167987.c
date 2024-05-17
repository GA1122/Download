bool ShouldUseClientLoFiForRequest(
    const ResourceRequest& request,
    WebURLRequest::PreviewsState frame_previews_state) {
  if (request.GetPreviewsState() != WebURLRequest::kPreviewsUnspecified)
    return request.GetPreviewsState() & WebURLRequest::kClientLoFiOn;

  if (!(frame_previews_state & WebURLRequest::kClientLoFiOn))
    return false;

  if (frame_previews_state & WebURLRequest::kServerLoFiOn)
    return request.Url().ProtocolIs("https");

  return true;
}
