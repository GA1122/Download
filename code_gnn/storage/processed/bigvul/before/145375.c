  static void Save(const v8::FunctionCallbackInfo<v8::Value>& info) {
    CHECK(info.Length() == 2 && info[0]->IsString() && info[1]->IsObject());
    SaveImpl(*v8::String::Utf8Value(info[0]),
             info[1],
             info.GetIsolate()->GetCurrentContext());
  }
