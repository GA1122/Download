JSObject* createReferenceError(ExecState* exec, const UString& message)
{
    return createReferenceError(exec->lexicalGlobalObject(), message);
}
