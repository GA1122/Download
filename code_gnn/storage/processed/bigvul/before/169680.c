base::Value* V8ValueConverterImpl::FromV8Array(
    v8::Local<v8::Array> val,
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
    if (strategy_->FromV8Array(val, &out, isolate, callback))
      return out;
  }

  base::ListValue* result = new base::ListValue();

  for (uint32_t i = 0; i < val->Length(); ++i) {
    v8::TryCatch try_catch(isolate);
    v8::Local<v8::Value> child_v8 = val->Get(i);
    if (try_catch.HasCaught()) {
      LOG(ERROR) << "Getter for index " << i << " threw an exception.";
      child_v8 = v8::Null(isolate);
    }

    if (!val->HasRealIndexedProperty(i)) {
      result->Append(base::Value::CreateNullValue());
      continue;
    }

    base::Value* child = FromV8ValueImpl(state, child_v8, isolate);
    if (child)
      result->Append(child);
    else
      result->Append(base::Value::CreateNullValue());
  }
  return result;
}
