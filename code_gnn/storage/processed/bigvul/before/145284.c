v8::Local<v8::Value> LanguageDetectionResult::ToValue(ScriptContext* context) {
  base::DictionaryValue dict_value;
  dict_value.SetBoolean("isReliable", is_reliable);
  scoped_ptr<base::ListValue> languages_list(new base::ListValue());
  for (const auto& language : languages)
    languages_list->Append(language->ToDictionary());
  dict_value.Set("languages", std::move(languages_list));

  v8::Local<v8::Context> v8_context = context->v8_context();
  v8::Isolate* isolate = v8_context->GetIsolate();
  v8::EscapableHandleScope handle_scope(isolate);

  scoped_ptr<content::V8ValueConverter> converter(
      content::V8ValueConverter::create());
  v8::Local<v8::Value> result = converter->ToV8Value(&dict_value, v8_context);
  return handle_scope.Escape(result);
}
