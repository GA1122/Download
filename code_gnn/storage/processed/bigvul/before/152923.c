void DevToolsClient::DidClearWindowObject() {
  if (!compatibility_script_.empty())
    render_frame()->ExecuteJavaScript(base::UTF8ToUTF16(compatibility_script_));
}
