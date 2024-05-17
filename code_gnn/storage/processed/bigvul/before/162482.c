void ImageResource::ResponseReceived(
    const ResourceResponse& response,
    std::unique_ptr<WebDataConsumerHandle> handle) {
  DCHECK(!handle);
  DCHECK(!multipart_parser_);
  if (response.IsMultipart() && !response.MultipartBoundary().IsEmpty()) {
    multipart_parser_ = new MultipartImageResourceParser(
        response, response.MultipartBoundary(), this);
  }

  Resource::ResponseReceived(response, std::move(handle));

  device_pixel_ratio_header_value_ =
      GetResponse()
          .HttpHeaderField(HTTPNames::Content_DPR)
          .ToFloat(&has_device_pixel_ratio_header_value_);
  if (!has_device_pixel_ratio_header_value_ ||
      device_pixel_ratio_header_value_ <= 0.0) {
    device_pixel_ratio_header_value_ = 1.0;
    has_device_pixel_ratio_header_value_ = false;
  }

  if (placeholder_option_ ==
          PlaceholderOption::kShowAndReloadPlaceholderAlways &&
      IsEntireResource(GetResponse())) {
    if (GetResponse().HttpStatusCode() < 400 ||
        GetResponse().HttpStatusCode() >= 600) {
      placeholder_option_ = PlaceholderOption::kDoNotReloadPlaceholder;
    } else {
      placeholder_option_ = PlaceholderOption::kReloadPlaceholderOnDecodeError;
    }
  }

  if (HasServerLoFiResponseHeaders(GetResponse())) {
    SetPreviewsState(GetResourceRequest().GetPreviewsState() |
                     WebURLRequest::kServerLoFiOn);
  } else if (GetResourceRequest().GetPreviewsState() &
             WebURLRequest::kServerLoFiOn) {
    WebURLRequest::PreviewsState new_previews_state =
        GetResourceRequest().GetPreviewsState();

    new_previews_state &= ~WebURLRequest::kServerLoFiOn;
    if (new_previews_state == WebURLRequest::kPreviewsUnspecified)
      new_previews_state = WebURLRequest::kPreviewsOff;

    SetPreviewsState(new_previews_state);
  }
}
