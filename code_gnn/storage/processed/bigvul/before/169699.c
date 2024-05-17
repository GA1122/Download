v8::Local<v8::Value> V8ValueConverterImpl::ToV8ValueImpl(
    v8::Isolate* isolate,
    v8::Local<v8::Object> creation_context,
    const base::Value* value) const {
  CHECK(value);
  switch (value->GetType()) {
    case base::Value::TYPE_NULL:
      return v8::Null(isolate);

    case base::Value::TYPE_BOOLEAN: {
      bool val = false;
      CHECK(value->GetAsBoolean(&val));
      return v8::Boolean::New(isolate, val);
    }

    case base::Value::TYPE_INTEGER: {
      int val = 0;
      CHECK(value->GetAsInteger(&val));
      return v8::Integer::New(isolate, val);
    }

    case base::Value::TYPE_DOUBLE: {
      double val = 0.0;
      CHECK(value->GetAsDouble(&val));
      return v8::Number::New(isolate, val);
    }

    case base::Value::TYPE_STRING: {
      std::string val;
      CHECK(value->GetAsString(&val));
      return v8::String::NewFromUtf8(
          isolate, val.c_str(), v8::String::kNormalString, val.length());
    }

    case base::Value::TYPE_LIST:
      return ToV8Array(isolate,
                       creation_context,
                       static_cast<const base::ListValue*>(value));

    case base::Value::TYPE_DICTIONARY:
      return ToV8Object(isolate,
                        creation_context,
                        static_cast<const base::DictionaryValue*>(value));

    case base::Value::TYPE_BINARY:
      return ToArrayBuffer(isolate,
                           creation_context,
                           static_cast<const base::BinaryValue*>(value));

    default:
      LOG(ERROR) << "Unexpected value type: " << value->GetType();
      return v8::Null(isolate);
  }
}
