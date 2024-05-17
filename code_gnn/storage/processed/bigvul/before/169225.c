bool ExecuteScriptWithoutUserGestureAndExtractBool(
    const ToRenderFrameHost& adapter,
    const std::string& script,
    bool* result) {
  DCHECK(result);
  std::unique_ptr<base::Value> value;
  return ExecuteScriptHelper(adapter.render_frame_host(), script, false,
                             &value) &&
         value && value->GetAsBoolean(result);
}
