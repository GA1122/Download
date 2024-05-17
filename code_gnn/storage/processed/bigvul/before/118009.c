V8Proxy* V8Proxy::retrieve(ScriptExecutionContext* context)
{
    if (!context || !context->isDocument())
        return 0;
    return retrieve(static_cast<Document*>(context)->frame());
}
