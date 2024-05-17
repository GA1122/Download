void RenderFrameImpl::JavaScriptExecuteRequest(
    const base::string16& javascript,
    JavaScriptExecuteRequestCallback callback) {
  TRACE_EVENT_INSTANT0("test_tracing", "JavaScriptExecuteRequest",
                       TRACE_EVENT_SCOPE_THREAD);

  base::WeakPtr<RenderFrameImpl> weak_this = weak_factory_.GetWeakPtr();

  v8::HandleScope handle_scope(v8::Isolate::GetCurrent());
  v8::Local<v8::Value> result = frame_->ExecuteScriptAndReturnValue(
      WebScriptSource(WebString::FromUTF16(javascript)));

  if (!weak_this)
    return;

  std::move(callback).Run(GetJavaScriptExecutionResult(result));
}
