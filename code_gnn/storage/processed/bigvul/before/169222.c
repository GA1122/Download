bool ExecuteScriptInIsolatedWorldHelper(RenderFrameHost* render_frame_host,
                                        const int world_id,
                                        const std::string& script,
                                        std::unique_ptr<base::Value>* result) {
  DOMMessageQueue dom_message_queue(
      WebContents::FromRenderFrameHost(render_frame_host));
  render_frame_host->ExecuteJavaScriptInIsolatedWorld(
      base::UTF8ToUTF16(script),
      content::RenderFrameHost::JavaScriptResultCallback(), world_id);
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
