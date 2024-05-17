bool DocumentLoader::ShouldContinueForResponse() const {
  if (substitute_data_.IsValid())
    return true;

  int status_code = response_.HttpStatusCode();
  if (status_code == 204 || status_code == 205) {
    return false;
  }

  if (IsContentDispositionAttachment(
          response_.HttpHeaderField(http_names::kContentDisposition))) {
    return false;
  }

  if (!CanShowMIMEType(response_.MimeType(), frame_))
    return false;
  return true;
}
