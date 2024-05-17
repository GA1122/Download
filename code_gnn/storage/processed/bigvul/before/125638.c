void RenderViewHostImpl::OnAddMessageToConsole(
    int32 level,
    const string16& message,
    int32 line_no,
    const string16& source_id) {
  if (delegate_->AddMessageToConsole(level, message, line_no, source_id))
    return;
  int32 resolved_level =
      (enabled_bindings_ & BINDINGS_POLICY_WEB_UI) ? level : 0;

  if (resolved_level >= ::logging::GetMinLogLevel()) {
    logging::LogMessage("CONSOLE", line_no, resolved_level).stream() << "\"" <<
        message << "\", source: " << source_id << " (" << line_no << ")";
  }
}
