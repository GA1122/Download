JSObject* throwTypeError(ExecState* exec)
{
    return throwError(exec, createTypeError(exec, "Type error"));
}
