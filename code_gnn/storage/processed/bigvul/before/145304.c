PrintNativeHandler::PrintNativeHandler(ScriptContext* context)
    : ObjectBackedNativeHandler(context) {
  RouteFunction("Print",
                base::Bind(&PrintNativeHandler::Print, base::Unretained(this)));
}
