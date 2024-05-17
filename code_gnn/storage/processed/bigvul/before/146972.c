void WebLocalFrameImpl::ExecuteScript(const WebScriptSource& source) {
  DCHECK(GetFrame());
  TextPosition position(OrdinalNumber::FromOneBasedInt(source.start_line),
                        OrdinalNumber::First());
  v8::HandleScope handle_scope(ToIsolate(GetFrame()));
  GetFrame()->GetScriptController().ExecuteScriptInMainWorld(
      ScriptSourceCode(source.code, source.url, position));
}
