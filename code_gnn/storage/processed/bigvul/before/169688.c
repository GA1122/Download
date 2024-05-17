base::Value* V8ValueConverterImpl::FromV8ValueImpl(
    FromV8ValueState* state,
    v8::Local<v8::Value> val,
    v8::Isolate* isolate) const {
  CHECK(!val.IsEmpty());

  FromV8ValueState::Level state_level(state);
  if (state->HasReachedMaxRecursionDepth())
    return NULL;

  if (val->IsNull())
    return base::Value::CreateNullValue().release();

  if (val->IsBoolean())
    return new base::FundamentalValue(val->ToBoolean(isolate)->Value());

  if (val->IsNumber() && strategy_) {
    base::Value* out = NULL;
    if (strategy_->FromV8Number(val.As<v8::Number>(), &out))
      return out;
  }

  if (val->IsInt32())
    return new base::FundamentalValue(val->ToInt32(isolate)->Value());

  if (val->IsNumber()) {
    double val_as_double = val.As<v8::Number>()->Value();
    if (!std::isfinite(val_as_double))
      return NULL;
    return new base::FundamentalValue(val_as_double);
  }

  if (val->IsString()) {
    v8::String::Utf8Value utf8(val);
    return new base::StringValue(std::string(*utf8, utf8.length()));
  }

  if (val->IsUndefined()) {
    if (strategy_) {
      base::Value* out = NULL;
      if (strategy_->FromV8Undefined(&out))
        return out;
    }
    return NULL;
  }

  if (val->IsDate()) {
    if (!date_allowed_)
      return FromV8Object(val->ToObject(isolate), state, isolate);
    v8::Date* date = v8::Date::Cast(*val);
    return new base::FundamentalValue(date->ValueOf() / 1000.0);
  }

  if (val->IsRegExp()) {
    if (!reg_exp_allowed_)
      return FromV8Object(val.As<v8::Object>(), state, isolate);
    return new base::StringValue(*v8::String::Utf8Value(val));
  }

  if (val->IsArray())
    return FromV8Array(val.As<v8::Array>(), state, isolate);

  if (val->IsFunction()) {
    if (!function_allowed_)
      return NULL;
    return FromV8Object(val.As<v8::Object>(), state, isolate);
  }

  if (val->IsArrayBuffer() || val->IsArrayBufferView())
    return FromV8ArrayBuffer(val.As<v8::Object>(), isolate);

  if (val->IsObject())
    return FromV8Object(val.As<v8::Object>(), state, isolate);

  LOG(ERROR) << "Unexpected v8 value type encountered.";
  return NULL;
}
