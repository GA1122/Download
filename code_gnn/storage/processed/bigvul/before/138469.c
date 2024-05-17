bool Microtask::performingCheckpoint(v8::Isolate* isolate)
{
    return V8PerIsolateData::from(isolate)->performingMicrotaskCheckpoint();
}
