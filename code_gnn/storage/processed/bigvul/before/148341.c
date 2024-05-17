bool WebContentsImpl::DidAddMessageToConsole(int32_t level,
                                             const base::string16& message,
                                             int32_t line_no,
                                             const base::string16& source_id) {
  if (!delegate_)
    return false;
  return delegate_->DidAddMessageToConsole(this, level, message, line_no,
                                           source_id);
}
