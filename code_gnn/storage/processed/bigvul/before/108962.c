void RenderViewImpl::OnScriptEvalRequest(const string16& frame_xpath,
                                         const string16& jscript,
                                         int id,
                                         bool notify_result) {
  TRACE_EVENT_INSTANT0("test_tracing", "OnScriptEvalRequest");
  EvaluateScript(frame_xpath, jscript, id, notify_result);
}
