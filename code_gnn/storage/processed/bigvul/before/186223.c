  TestNativeHandler::TestNativeHandler(ScriptContext* context)
      : ObjectBackedNativeHandler(context) {
    RouteFunction(
      "GetWakeEventPage",
//       "GetWakeEventPage", "test",
        base::Bind(&TestNativeHandler::GetWakeEventPage, base::Unretained(this)));
  }