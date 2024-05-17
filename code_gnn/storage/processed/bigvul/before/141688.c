void V8Console::monitorFunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ConsoleHelper helper(info);
    v8::Local<v8::Function> function;
    if (!helper.firstArgAsFunction().ToLocal(&function))
        return;
    v8::Local<v8::Value> name = function->GetName();
    if (!name->IsString() || !v8::Local<v8::String>::Cast(name)->Length())
        name = function->GetInferredName();
    String16 functionName = toProtocolStringWithTypeCheck(name);
    String16Builder builder;
    builder.append("console.log(\"function ");
    if (functionName.isEmpty())
        builder.append("(anonymous function)");
    else
        builder.append(functionName);
    builder.append(" called\" + (arguments.length > 0 ? \" with arguments: \" + Array.prototype.join.call(arguments, \", \") : \"\")) && false");
    setFunctionBreakpoint(helper, function, V8DebuggerAgentImpl::MonitorCommandBreakpointSource, builder.toString(), true);
}
