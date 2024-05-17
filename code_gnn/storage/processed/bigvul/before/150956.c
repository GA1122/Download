static void installPrivateScript(v8::Isolate* isolate, String className) {
  ScriptState* scriptState = ScriptState::current(isolate);
  int compiledScriptCount = 0;
#ifndef NDEBUG
  for (size_t index = 0;
       index < WTF_ARRAY_LENGTH(kPrivateScriptSourcesForTesting); index++) {
    if (className == kPrivateScriptSourcesForTesting[index].className) {
      compileAndRunPrivateScript(
          scriptState, kPrivateScriptSourcesForTesting[index].scriptClassName,
          kPrivateScriptSourcesForTesting[index].source,
          kPrivateScriptSourcesForTesting[index].size);
      compiledScriptCount++;
    }
  }
#endif

  for (size_t index = 0; index < WTF_ARRAY_LENGTH(kPrivateScriptSources);
       index++) {
    if (className == kPrivateScriptSources[index].className) {
      String resourceData =
          loadResourceAsASCIIString(kPrivateScriptSources[index].resourceFile);
      compileAndRunPrivateScript(
          scriptState, kPrivateScriptSources[index].scriptClassName,
          resourceData.utf8().data(), resourceData.length());
      compiledScriptCount++;
    }
  }

  if (!compiledScriptCount) {
    LOG(FATAL)
        << "Private script error: Target source code was not found. (Class "
           "name = "
        << className.utf8().data() << ")";
  }
}
