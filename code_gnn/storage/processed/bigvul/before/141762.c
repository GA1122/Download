void V8InjectedScriptHost::formatAccessorsAsProperties(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    DCHECK_EQ(info.Length(), 2);
    info.GetReturnValue().Set(false);
    if (!info[1]->IsFunction())
        return;
    if (info[1].As<v8::Function>()->ScriptId() != v8::UnboundScript::kNoScriptId)
        return;
    info.GetReturnValue().Set(unwrapInspector(info)->client()->formatAccessorsAsProperties(info[0]));
}
