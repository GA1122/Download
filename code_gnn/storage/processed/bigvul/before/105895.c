JSObject* throwError(ExecState* exec, JSObject* error)
{
    Interpreter::addStackTraceIfNecessary(exec, error);
    exec->globalData().exception = error;
    return error;
}
