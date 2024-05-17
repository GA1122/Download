PassRefPtr<ScriptHeapSnapshot> ScriptProfiler::takeHeapSnapshot(const String& title, HeapSnapshotProgress* control)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HeapProfiler* profiler = isolate->GetHeapProfiler();
    if (!profiler)
        return 0;
    v8::HandleScope handleScope(isolate);
    ASSERT(control);
    ActivityControlAdapter adapter(control);
    GlobalObjectNameResolver resolver;
    const v8::HeapSnapshot* snapshot = profiler->TakeHeapSnapshot(v8String(title, isolate), &adapter, &resolver);
    return snapshot ? ScriptHeapSnapshot::create(snapshot) : 0;
}
