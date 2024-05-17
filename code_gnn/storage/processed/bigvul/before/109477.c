unsigned ScriptProfiler::getHeapObjectId(const ScriptValue& value)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HeapProfiler* profiler = isolate->GetHeapProfiler();
    v8::SnapshotObjectId id = profiler->GetObjectId(value.v8Value());
    return id;
}
