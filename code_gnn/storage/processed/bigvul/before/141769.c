V8InspectorImpl* unwrapInspector(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    DCHECK(!info.Data().IsEmpty());
    DCHECK(info.Data()->IsExternal());
    V8InspectorImpl* inspector = static_cast<V8InspectorImpl*>(info.Data().As<v8::External>()->Value());
    DCHECK(inspector);
    return inspector;
}
