void ScriptProfiler::collectGarbage()
{
    v8::V8::LowMemoryNotification();
}
