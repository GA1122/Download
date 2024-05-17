bool RenderFrameImpl::RunJavaScriptDialog(JavaScriptDialogType type,
                                          const base::string16& message,
                                          const base::string16& default_value,
                                          const GURL& frame_url,
                                          base::string16* result) {
  if (suppress_further_dialogs_)
    return false;

  int32_t message_length = static_cast<int32_t>(message.length());
  if (WebUserGestureIndicator::ProcessedUserGestureSinceLoad(frame_)) {
    UMA_HISTOGRAM_COUNTS("JSDialogs.CharacterCount.UserGestureSinceLoad",
                         message_length);
  } else {
    UMA_HISTOGRAM_COUNTS("JSDialogs.CharacterCount.NoUserGestureSinceLoad",
                         message_length);
  }

  bool success = false;
  base::string16 result_temp;
  if (!result)
    result = &result_temp;

  Send(new FrameHostMsg_RunJavaScriptDialog(routing_id_, message, default_value,
                                            frame_url, type, &success, result));
  return success;
}
