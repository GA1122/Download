base::Value* V8ValueConverterImpl::FromV8Object(
    v8::Local<v8::Object> val,
    FromV8ValueState* state,
    v8::Isolate* isolate) const {
  if (!state->UpdateAndCheckUniqueness(val))
    return base::Value::CreateNullValue().release();

  std::unique_ptr<v8::Context::Scope> scope;
  if (!val->CreationContext().IsEmpty() &&
      val->CreationContext() != isolate->GetCurrentContext())
    scope.reset(new v8::Context::Scope(val->CreationContext()));

  if (strategy_) {
    V8ValueConverter::Strategy::FromV8ValueCallback callback =
        base::Bind(&V8ValueConverterImpl::FromV8ValueImpl,
                   base::Unretained(this),
                   base::Unretained(state));
    base::Value* out = NULL;
    if (strategy_->FromV8Object(val, &out, isolate, callback))
      return out;
  }

  if (val->InternalFieldCount())
    return new base::DictionaryValue();

  std::unique_ptr<base::DictionaryValue> result(new base::DictionaryValue());
  v8::Local<v8::Array> property_names(val->GetOwnPropertyNames());

  for (uint32_t i = 0; i < property_names->Length(); ++i) {
    v8::Local<v8::Value> key(property_names->Get(i));

    if (!key->IsString() &&
        !key->IsNumber()) {
      NOTREACHED() << "Key \"" << *v8::String::Utf8Value(key) << "\" "
                      "is neither a string nor a number";
      continue;
    }

    v8::String::Utf8Value name_utf8(key);

    v8::TryCatch try_catch(isolate);
    v8::Local<v8::Value> child_v8 = val->Get(key);

    if (try_catch.HasCaught()) {
      LOG(WARNING) << "Getter for property " << *name_utf8
                   << " threw an exception.";
      child_v8 = v8::Null(isolate);
    }

    std::unique_ptr<base::Value> child(
        FromV8ValueImpl(state, child_v8, isolate));
    if (!child)
      continue;

    if (strip_null_from_objects_ && child->IsType(base::Value::TYPE_NULL))
      continue;

    result->SetWithoutPathExpansion(std::string(*name_utf8, name_utf8.length()),
                                    child.release());
  }

  return result.release();
}
