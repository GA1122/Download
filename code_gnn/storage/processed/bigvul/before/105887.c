JSObject* createSyntaxError(ExecState* exec, const UString& message)
{
    return createSyntaxError(exec->lexicalGlobalObject(), message);
}
