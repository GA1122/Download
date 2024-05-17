void ScriptProfiler::initialize()
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HeapProfiler* profiler = isolate->GetHeapProfiler();
    if (profiler)
        profiler->SetWrapperClassInfoProvider(v8DOMNodeClassId, &retainedDOMInfo);
}
