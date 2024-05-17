  void TestWeirdType(const V8ValueConverterImpl& converter,
                     v8::Local<v8::Value> val,
                     base::Value::Type expected_type,
                     std::unique_ptr<base::Value> expected_value) {
    v8::Local<v8::Context> context =
        v8::Local<v8::Context>::New(isolate_, context_);
    std::unique_ptr<base::Value> raw(converter.FromV8Value(val, context));

    if (expected_value) {
      ASSERT_TRUE(raw.get());
      EXPECT_TRUE(expected_value->Equals(raw.get()));
      EXPECT_EQ(expected_type, raw->GetType());
    } else {
      EXPECT_FALSE(raw.get());
    }

    v8::Local<v8::Object> object(v8::Object::New(isolate_));
    object->Set(v8::String::NewFromUtf8(isolate_, "test"), val);
    std::unique_ptr<base::DictionaryValue> dictionary(
        static_cast<base::DictionaryValue*>(
            converter.FromV8Value(object, context)));
    ASSERT_TRUE(dictionary.get());

    if (expected_value) {
      base::Value* temp = NULL;
      ASSERT_TRUE(dictionary->Get("test", &temp));
      EXPECT_EQ(expected_type, temp->GetType());
      EXPECT_TRUE(expected_value->Equals(temp));
    } else {
      EXPECT_FALSE(dictionary->HasKey("test"));
    }

    v8::Local<v8::Array> array(v8::Array::New(isolate_));
    array->Set(0, val);
    std::unique_ptr<base::ListValue> list(
        static_cast<base::ListValue*>(converter.FromV8Value(array, context)));
    ASSERT_TRUE(list.get());
    if (expected_value) {
      base::Value* temp = NULL;
      ASSERT_TRUE(list->Get(0, &temp));
      EXPECT_EQ(expected_type, temp->GetType());
      EXPECT_TRUE(expected_value->Equals(temp));
    } else {
      base::Value* temp = NULL;
      ASSERT_TRUE(list->Get(0, &temp));
      EXPECT_EQ(base::Value::TYPE_NULL, temp->GetType());
    }
  }
