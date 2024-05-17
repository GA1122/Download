void V8Console::lastEvaluationResultCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ConsoleHelper helper(info);
    InspectedContext* context = helper.ensureInspectedContext();
    if (!context)
        return;
    if (InjectedScript* injectedScript = context->getInjectedScript())
        info.GetReturnValue().Set(injectedScript->lastEvaluationResult());
}
