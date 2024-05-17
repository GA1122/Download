void Microtask::enqueueMicrotask(PassOwnPtr<WebThread::Task> callback)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    isolate->EnqueueMicrotask(&microtaskFunctionCallback, callback.leakPtr());
}
