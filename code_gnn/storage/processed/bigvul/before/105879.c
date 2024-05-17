JSObject* createError(ExecState* exec, const UString& message)
{
    return createError(exec->lexicalGlobalObject(), message);
}
