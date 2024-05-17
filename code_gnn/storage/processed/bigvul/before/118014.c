V8BindingPerContextData* V8Proxy::retrievePerContextData(Frame* frame)
{
    V8IsolatedContext* isolatedContext;
    if (UNLIKELY(!!(isolatedContext = V8IsolatedContext::getEntered())))
        return isolatedContext->perContextData();
    return V8Proxy::retrieve(frame)->windowShell()->perContextData();
}
