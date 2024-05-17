bool WebContentsImpl::AddMessageToConsole(int32 level,
                                          const base::string16& message,
                                          int32 line_no,
                                          const base::string16& source_id) {
  if (!delegate_)
    return false;
  return delegate_->AddMessageToConsole(this, level, message, line_no,
                                        source_id);
}
