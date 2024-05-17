v8::Local<v8::Value> ModuleSystem::RunString(v8::Local<v8::String> code,
                                             v8::Local<v8::String> name) {
  return context_->RunScript(
      name, code, base::Bind(&ExceptionHandler::HandleUncaughtException,
                             base::Unretained(exception_handler_.get())));
}
