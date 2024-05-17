ScriptObject ScriptProfiler::objectByHeapObjectId(unsigned id)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HeapProfiler* profiler = isolate->GetHeapProfiler();
    if (!profiler)
        return ScriptObject();
    const v8::HeapGraphNode* node = 0;
    for (int i = 0, l = profiler->GetSnapshotCount(); i < l; ++i) {
        const v8::HeapSnapshot* snapshot = profiler->GetHeapSnapshot(i);
        node = snapshot->GetNodeById(id);
        if (node)
            break;
    }
    if (!node)
        return ScriptObject();

    v8::HandleScope handleScope(isolate);
    v8::Handle<v8::Value> value = node->GetHeapValue();
    if (!value->IsObject())
        return ScriptObject();

    v8::Handle<v8::Object> object = value.As<v8::Object>();

    if (object->InternalFieldCount() >= v8DefaultWrapperInternalFieldCount) {
        v8::Handle<v8::Value> wrapper = object->GetInternalField(v8DOMWrapperObjectIndex);
        if (!wrapper.IsEmpty() && wrapper->IsUndefined())
            return ScriptObject();
    }

    ScriptState* scriptState = ScriptState::forContext(object->CreationContext());
    return ScriptObject(scriptState, object);
}
