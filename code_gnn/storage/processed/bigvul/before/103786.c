void RenderView::EvaluateScript(const string16& frame_xpath,
                                const string16& script,
                                int id,
                                bool notify_result) {
  v8::Handle<v8::Value> result;
  WebFrame* web_frame = GetChildFrame(UTF16ToWideHack(frame_xpath));
  if (web_frame)
    result = web_frame->executeScriptAndReturnValue(WebScriptSource(script));
  if (notify_result) {
    ListValue list;
    if (!result.IsEmpty() && web_frame) {
      v8::HandleScope handle_scope;
      v8::Local<v8::Context> context = web_frame->mainWorldScriptContext();
      v8::Context::Scope context_scope(context);
      V8ValueConverter converter;
      converter.set_allow_date(true);
      converter.set_allow_regexp(true);
      list.Set(0, converter.FromV8Value(result, context));
    } else {
      list.Set(0, Value::CreateNullValue());
    }
    Send(new ViewHostMsg_ScriptEvalResponse(routing_id_, id, list));
  }
}
