std::unique_ptr<TracedValue> InspectorFunctionCallEvent::Data(
    ExecutionContext* context,
    const v8::Local<v8::Function>& function) {
  std::unique_ptr<TracedValue> value = TracedValue::Create();
  if (LocalFrame* frame = FrameForExecutionContext(context))
    value->SetString("frame", ToHexString(frame));

  if (function.IsEmpty())
    return value;

  v8::Local<v8::Function> original_function = GetBoundFunction(function);
  v8::Local<v8::Value> function_name = original_function->GetDebugName();
  if (!function_name.IsEmpty() && function_name->IsString()) {
    value->SetString("functionName",
                     ToCoreString(function_name.As<v8::String>()));
  }
  std::unique_ptr<SourceLocation> location =
      SourceLocation::FromFunction(original_function);
  value->SetString("scriptId", String::Number(location->ScriptId()));
  value->SetString("url", location->Url());
  value->SetInteger("lineNumber", location->LineNumber());
  return value;
}
