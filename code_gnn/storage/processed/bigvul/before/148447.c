void WebContentsImpl::NotifyBeforeFormRepostWarningShow() {
  for (auto& observer : observers_)
    observer.BeforeFormRepostWarningShow();
}
