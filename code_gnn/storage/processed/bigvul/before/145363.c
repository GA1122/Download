  static void Apply(const v8::FunctionCallbackInfo<v8::Value>& info) {
    CHECK(info.Length() == 5 && info[0]->IsFunction() &&   
          info[2]->IsObject() &&   
          info[3]->IsInt32() &&    
          info[4]->IsInt32());     
    v8::Local<v8::Function> function = info[0].As<v8::Function>();
    v8::Local<v8::Object> recv;
    if (info[1]->IsObject()) {
      recv = v8::Local<v8::Object>::Cast(info[1]);
    } else if (info[1]->IsString()) {
      recv = v8::StringObject::New(v8::Local<v8::String>::Cast(info[1]))
                 .As<v8::Object>();
    } else {
      info.GetIsolate()->ThrowException(
          v8::Exception::TypeError(ToV8StringUnsafe(
              info.GetIsolate(),
              "The first argument is the receiver and must be an object")));
      return;
    }
    v8::Local<v8::Object> args = v8::Local<v8::Object>::Cast(info[2]);
    int first_arg_index = info[3].As<v8::Int32>()->Value();
    int args_length = info[4].As<v8::Int32>()->Value();

    v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
    int argc = args_length - first_arg_index;
    std::unique_ptr<v8::Local<v8::Value>[]> argv(
        new v8::Local<v8::Value>[argc]);
    for (int i = 0; i < argc; ++i) {
      CHECK(IsTrue(args->Has(context, i + first_arg_index)));
      if (!GetProperty(context, args, i + first_arg_index, &argv[i]))
        return;
    }

    v8::MicrotasksScope microtasks(
        info.GetIsolate(), v8::MicrotasksScope::kDoNotRunMicrotasks);
    v8::Local<v8::Value> return_value;
    if (function->Call(context, recv, argc, argv.get()).ToLocal(&return_value))
      info.GetReturnValue().Set(return_value);
  }
