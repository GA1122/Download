bool WebContentsImpl::IsSavable() {
  return contents_mime_type_ == "text/html" ||
         contents_mime_type_ == "text/xml" ||
         contents_mime_type_ == "application/xhtml+xml" ||
         contents_mime_type_ == "text/plain" ||
         contents_mime_type_ == "text/css" ||
         mime_util::IsSupportedJavascriptMimeType(contents_mime_type_);
}
