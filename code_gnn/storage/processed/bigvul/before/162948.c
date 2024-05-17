bool MockWebContentsDelegate::DidAddMessageToConsole(
    WebContents* source,
    int32_t level,
    const base::string16& message,
    int32_t line_no,
    const base::string16& source_id) {
  DCHECK(source == web_contents_);

  if (level == logging::LOG_ERROR || level == logging::LOG_WARNING)
    test_->OnReceivedConsoleError();
  return false;
}
