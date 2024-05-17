void EventBindings::DetachEventHandler(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  CHECK_EQ(2, args.Length());
  CHECK(args[0]->IsString());
  CHECK(args[1]->IsBoolean());
  DetachEvent(*v8::String::Utf8Value(args[0]), args[1]->BooleanValue());
}
