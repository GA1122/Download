bool RenderFrameImpl::RunJavaScriptDialog(JavaScriptDialogType type,
                                          const base::string16& message,
                                          const base::string16& default_value,
                                          base::string16* result) {
  if (suppress_further_dialogs_)
    return false;

  int32_t message_length = static_cast<int32_t>(message.length());
  if (WebUserGestureIndicator::ProcessedUserGestureSinceLoad(frame_)) {
    UMA_HISTOGRAM_COUNTS_1M("JSDialogs.CharacterCount.UserGestureSinceLoad",
                            message_length);
  } else {
    UMA_HISTOGRAM_COUNTS_1M("JSDialogs.CharacterCount.NoUserGestureSinceLoad",
                            message_length);
  }

  if (is_main_frame_)
    UMA_HISTOGRAM_COUNTS_1M("JSDialogs.CharacterCount.MainFrame",
                            message_length);
  else
    UMA_HISTOGRAM_COUNTS_1M("JSDialogs.CharacterCount.Subframe",
                            message_length);

  const base::string16::size_type kMaxMessageSize = 10 * 1024;
  base::string16 truncated_message = message.substr(0, kMaxMessageSize);

  bool success = false;
  base::string16 result_temp;
  if (!result)
    result = &result_temp;

  Send(new FrameHostMsg_RunJavaScriptDialog(
      routing_id_, truncated_message, default_value, type, &success, result));
  return success;
}
