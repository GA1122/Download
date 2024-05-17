v8::Local<v8::Value> WebLocalFrameImpl::ExecuteScriptAndReturnValue(
    const WebScriptSource& source) {
  DCHECK(GetFrame());

  return GetFrame()
      ->GetScriptController()
      .ExecuteScriptInMainWorldAndReturnValue(source);
}
