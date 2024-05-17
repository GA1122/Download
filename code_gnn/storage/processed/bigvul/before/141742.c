void V8Debugger::handleV8AsyncTaskEvent(v8::Local<v8::Context> context, v8::Local<v8::Object> executionState, v8::Local<v8::Object> eventData)
{
    if (!m_maxAsyncCallStackDepth)
        return;

    String16 type = toProtocolStringWithTypeCheck(callInternalGetterFunction(eventData, "type"));
    String16 name = toProtocolStringWithTypeCheck(callInternalGetterFunction(eventData, "name"));
    int id = callInternalGetterFunction(eventData, "id")->ToInteger(m_isolate)->Value();
    void* ptr = reinterpret_cast<void*>(id * 4 + (name[0] == 'P' ? 2 : 0) + 1);
    if (type == v8AsyncTaskEventEnqueue)
        asyncTaskScheduled(name, ptr, false);
    else if (type == v8AsyncTaskEventWillHandle)
        asyncTaskStarted(ptr);
    else if (type == v8AsyncTaskEventDidHandle)
        asyncTaskFinished(ptr);
    else
        NOTREACHED();
}
