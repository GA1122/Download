void ImageResource::OnePartInMultipartReceived(
    const ResourceResponse& response) {
  DCHECK(multipart_parser_);

  SetResponse(response);
  if (multipart_parsing_state_ == MultipartParsingState::kWaitingForFirstPart) {
    multipart_parsing_state_ = MultipartParsingState::kParsingFirstPart;
    return;
  }
  UpdateImageAndClearBuffer();

  if (multipart_parsing_state_ == MultipartParsingState::kParsingFirstPart) {
    multipart_parsing_state_ = MultipartParsingState::kFinishedParsingFirstPart;
    if (!ErrorOccurred())
      SetStatus(ResourceStatus::kCached);
    NotifyFinished();
    if (Loader())
      Loader()->DidFinishLoadingFirstPartInMultipart();
  }
}
