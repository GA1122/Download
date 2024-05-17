UrlmonUrlRequestManager::UrlmonUrlRequestManager()
    : stopping_(false), calling_delegate_(0), notification_window_(NULL),
      privileged_mode_(false),
      container_(NULL) {
}
