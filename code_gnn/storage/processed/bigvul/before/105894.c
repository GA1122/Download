JSValue throwError(ExecState* exec, JSValue error)
{
    if (error.isObject())
        return throwError(exec, asObject(error));
    exec->globalData().exception = error;
    return error;
}
