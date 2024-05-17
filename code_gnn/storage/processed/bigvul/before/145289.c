bool ObjectBackedNativeHandler::ContextCanAccessObject(
    const v8::Local<v8::Context>& context,
    const v8::Local<v8::Object>& object,
    bool allow_null_context) {
  if (object->IsNull())
    return true;
  if (context == object->CreationContext())
    return true;
  ScriptContext* other_script_context =
      ScriptContextSet::GetContextByObject(object);
  if (!other_script_context || !other_script_context->web_frame())
    return allow_null_context;

  return blink::WebFrame::scriptCanAccess(other_script_context->web_frame());
}
