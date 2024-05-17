JSObject* createEvalError(ExecState* exec, const UString& message)
{
    return createEvalError(exec->lexicalGlobalObject(), message);
}
