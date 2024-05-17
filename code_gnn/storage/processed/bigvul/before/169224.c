bool ExecuteScriptWithoutUserGesture(const ToRenderFrameHost& adapter,
                                     const std::string& script) {
  return ExecuteScriptWithUserGestureControl(adapter.render_frame_host(),
                                             script, false);
}
