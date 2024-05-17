void RenderFrameImpl::JavaScriptExecuteRequestForTests(
    const base::string16& javascript,
    bool has_user_gesture,
    JavaScriptExecuteRequestForTestsCallback callback) {
  TRACE_EVENT_INSTANT0("test_tracing", "JavaScriptExecuteRequestForTests",
                       TRACE_EVENT_SCOPE_THREAD);

  base::WeakPtr<RenderFrameImpl> weak_this = weak_factory_.GetWeakPtr();

  base::Optional<blink::WebScopedUserGesture> gesture;
  if (has_user_gesture)
    gesture.emplace(frame_);

  v8::HandleScope handle_scope(blink::MainThreadIsolate());
  v8::Local<v8::Value> result = frame_->ExecuteScriptAndReturnValue(
      WebScriptSource(WebString::FromUTF16(javascript)));

  if (!weak_this)
    return;

  std::move(callback).Run(GetJavaScriptExecutionResult(result));
}
