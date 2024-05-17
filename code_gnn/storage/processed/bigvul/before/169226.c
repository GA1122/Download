bool ExecuteScriptWithoutUserGestureAndExtractDouble(
    const ToRenderFrameHost& adapter,
    const std::string& script,
    double* result) {
  DCHECK(result);
  std::unique_ptr<base::Value> value;
  return ExecuteScriptHelper(adapter.render_frame_host(), script, false,
                             &value) &&
         value && value->GetAsDouble(result);
}
