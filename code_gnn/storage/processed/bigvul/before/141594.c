void EventBindings::DetachFilteredEventHandler(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  CHECK_EQ(2, args.Length());
  CHECK(args[0]->IsInt32());
  CHECK(args[1]->IsBoolean());
  DetachFilteredEvent(args[0]->Int32Value(), args[1]->BooleanValue());
}
