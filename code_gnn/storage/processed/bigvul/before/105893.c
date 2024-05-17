bool hasErrorInfo(ExecState* exec, JSObject* error)
{
    return error->hasProperty(exec, Identifier(exec, linePropertyName))
        || error->hasProperty(exec, Identifier(exec, sourceURLPropertyName));
}
