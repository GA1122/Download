static v8::Local<v8::Object> classObjectOfPrivateScript(
    ScriptState* scriptState,
    String className) {
  ASSERT(scriptState->perContextData());
  ASSERT(scriptState->getExecutionContext());
  v8::Isolate* isolate = scriptState->isolate();
  v8::Local<v8::Value> compiledClass =
      scriptState->perContextData()->compiledPrivateScript(className);
  if (compiledClass.IsEmpty()) {
    v8::Local<v8::Value> installedClasses =
        scriptState->perContextData()->compiledPrivateScript(
            "PrivateScriptRunner");
    if (installedClasses.IsEmpty()) {
      installedClasses = installPrivateScriptRunner(isolate);
      scriptState->perContextData()->setCompiledPrivateScript(
          "PrivateScriptRunner", installedClasses);
    }
    RELEASE_ASSERT(!installedClasses.IsEmpty());
    RELEASE_ASSERT(installedClasses->IsObject());

    installPrivateScript(isolate, className);
    compiledClass =
        v8::Local<v8::Object>::Cast(installedClasses)
            ->Get(scriptState->context(), v8String(isolate, className))
            .ToLocalChecked();
    RELEASE_ASSERT(compiledClass->IsObject());
    scriptState->perContextData()->setCompiledPrivateScript(className,
                                                            compiledClass);
  }
  return v8::Local<v8::Object>::Cast(compiledClass);
}
