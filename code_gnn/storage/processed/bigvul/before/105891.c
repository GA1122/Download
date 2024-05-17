 JSObject* createURIError(ExecState* exec, const UString& message)
 {
     return createURIError(exec->lexicalGlobalObject(), message);
}
