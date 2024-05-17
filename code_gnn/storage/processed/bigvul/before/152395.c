void RenderFrameImpl::JavaScriptExecuteRequestInIsolatedWorld(
    const base::string16& javascript,
    int32_t world_id,
    JavaScriptExecuteRequestInIsolatedWorldCallback callback) {
  TRACE_EVENT_INSTANT0("test_tracing",
                       "JavaScriptExecuteRequestInIsolatedWorld",
                       TRACE_EVENT_SCOPE_THREAD);

  if (world_id <= ISOLATED_WORLD_ID_GLOBAL ||
      world_id > ISOLATED_WORLD_ID_MAX) {
    NOTREACHED();
    std::move(callback).Run(base::Value());
    return;
  }

  v8::HandleScope handle_scope(v8::Isolate::GetCurrent());
  WebScriptSource script = WebScriptSource(WebString::FromUTF16(javascript));
  JavaScriptIsolatedWorldRequest* request = new JavaScriptIsolatedWorldRequest(
      weak_factory_.GetWeakPtr(), std::move(callback));
  frame_->RequestExecuteScriptInIsolatedWorld(
      world_id, &script, 1, false, WebLocalFrame::kSynchronous, request);
}
