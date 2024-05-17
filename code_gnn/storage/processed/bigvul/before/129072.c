void DebuggerFunction::FormatErrorMessage(const std::string& format) {
  if (debuggee_.tab_id)
    error_ = ErrorUtils::FormatErrorMessage(
      format, keys::kTabTargetType, base::IntToString(*debuggee_.tab_id));
  else if (debuggee_.extension_id)
    error_ = ErrorUtils::FormatErrorMessage(
      format, keys::kBackgroundPageTargetType, *debuggee_.extension_id);
  else
    error_ = ErrorUtils::FormatErrorMessage(
      format, keys::kOpaqueTargetType, *debuggee_.target_id);
 }
