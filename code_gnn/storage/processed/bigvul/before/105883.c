JSObject* createRangeError(ExecState* exec, const UString& message)
{
    return createRangeError(exec->lexicalGlobalObject(), message);
}
