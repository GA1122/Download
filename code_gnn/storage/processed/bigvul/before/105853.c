UrlmonUrlRequest::UrlmonUrlRequest()
    : pending_read_size_(0),
      headers_received_(false),
      calling_delegate_(0),
      thread_(NULL),
      parent_window_(NULL),
      privileged_mode_(false),
      pending_(false),
      is_expecting_download_(true),
      cleanup_transaction_(false) {
  DVLOG(1) << __FUNCTION__ << me();
}
