void RenderFrameImpl::OnJavaScriptExecuteRequestForTests(
    const base::string16& jscript,
    int id,
    bool notify_result,
    bool has_user_gesture) {
  TRACE_EVENT_INSTANT0("test_tracing", "OnJavaScriptExecuteRequestForTests",
                       TRACE_EVENT_SCOPE_THREAD);

  scoped_ptr<blink::WebScopedUserGesture> gesture(
      has_user_gesture ? new blink::WebScopedUserGesture : nullptr);
  v8::HandleScope handle_scope(blink::mainThreadIsolate());
  v8::Local<v8::Value> result =
      frame_->executeScriptAndReturnValue(WebScriptSource(jscript));

  HandleJavascriptExecutionResult(jscript, id, notify_result, result);
}
