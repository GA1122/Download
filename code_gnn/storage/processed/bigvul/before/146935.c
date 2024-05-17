void WebLocalFrameImpl::CollectGarbage() {
  if (!GetFrame())
    return;
  if (!GetFrame()->GetSettings()->GetScriptEnabled())
    return;
  V8GCController::CollectGarbage(v8::Isolate::GetCurrent());
}
