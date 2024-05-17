v8::Local<v8::Value> WebLocalFrameImpl::ExecuteScriptAndReturnValue(
    const WebScriptSource& source) {
  DCHECK(GetFrame());

  TextPosition position(OrdinalNumber::FromOneBasedInt(source.start_line),
                        OrdinalNumber::First());
  return GetFrame()
      ->GetScriptController()
      .ExecuteScriptInMainWorldAndReturnValue(
          ScriptSourceCode(source.code, source.url, position));
}
