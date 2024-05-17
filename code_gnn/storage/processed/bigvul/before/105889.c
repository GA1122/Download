JSObject* createTypeError(ExecState* exec, const UString& message)
{
     return createTypeError(exec->lexicalGlobalObject(), message);
 }
