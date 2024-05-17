bool ExecuteScriptHelper(RenderFrameHost* render_frame_host,
                         const std::string& script,
                         bool user_gesture,
                         std::unique_ptr<base::Value>* result) {
  DOMMessageQueue dom_message_queue(
      WebContents::FromRenderFrameHost(render_frame_host));
  if (user_gesture) {
    render_frame_host->ExecuteJavaScriptWithUserGestureForTests(
        base::UTF8ToUTF16(script));
  } else {
    render_frame_host->ExecuteJavaScriptForTests(base::UTF8ToUTF16(script));
  }
  std::string json;
  if (!dom_message_queue.WaitForMessage(&json)) {
    DLOG(ERROR) << "Cannot communicate with DOMMessageQueue.";
    return false;
  }

  if (!result)
    return true;

  base::JSONReader reader(base::JSON_ALLOW_TRAILING_COMMAS);
  *result = reader.ReadToValue(json);
  if (!*result) {
    DLOG(ERROR) << reader.GetErrorMessage();
    return false;
  }

  return true;
}
