void TestNativeHandler::GetWakeEventPage(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
  CHECK_EQ(0, args.Length());
  args.GetReturnValue().Set(WakeEventPage::Get()->GetForContext(context()));
}
