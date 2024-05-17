void SetExportsProperty(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Local<v8::Object> obj = args.This();
  CHECK_EQ(2, args.Length());
  CHECK(args[0]->IsString());
  v8::Maybe<bool> result =
      obj->DefineOwnProperty(args.GetIsolate()->GetCurrentContext(),
                             args[0]->ToString(), args[1], v8::ReadOnly);
  if (!result.FromMaybe(false))
    LOG(ERROR) << "Failed to set private property on the export.";
}
