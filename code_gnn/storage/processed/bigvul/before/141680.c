void V8Console::inspectedObject(const v8::FunctionCallbackInfo<v8::Value>& info, unsigned num)
{
    DCHECK(num < V8InspectorSessionImpl::kInspectedObjectBufferSize);
    ConsoleHelper helper(info);
    if (V8InspectorSessionImpl* session = helper.currentSession()) {
        V8InspectorSession::Inspectable* object = session->inspectedObject(num);
        v8::Isolate* isolate = info.GetIsolate();
        if (object)
            info.GetReturnValue().Set(object->get(isolate->GetCurrentContext()));
        else
            info.GetReturnValue().Set(v8::Undefined(isolate));
    }
}
