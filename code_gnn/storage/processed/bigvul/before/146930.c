v8::Local<v8::Value> WebLocalFrameImpl::CallFunctionEvenIfScriptDisabled(
    v8::Local<v8::Function> function,
    v8::Local<v8::Value> receiver,
    int argc,
    v8::Local<v8::Value> argv[]) {
  DCHECK(GetFrame());
  v8::Local<v8::Value> result;
  if (!V8ScriptRunner::CallFunction(
           function, GetFrame()->GetDocument(), receiver, argc,
           static_cast<v8::Local<v8::Value>*>(argv), ToIsolate(GetFrame()))
           .ToLocal(&result))
    return v8::Local<v8::Value>();
  return result;
}
