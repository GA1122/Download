bool ExecuteScriptAndExtractBool(const ToRenderFrameHost& adapter,
                                 const std::string& script, bool* result) {
  DCHECK(result);
  std::unique_ptr<base::Value> value;
  if (!ExecuteScriptHelper(adapter.render_frame_host(), script, true, &value) ||
      !value.get()) {
    return false;
  }

  return value->GetAsBoolean(result);
}
