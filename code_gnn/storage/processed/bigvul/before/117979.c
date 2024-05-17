v8::Handle<v8::Value> V8Proxy::checkNewLegal(const v8::Arguments& args)
{
    if (ConstructorMode::current() == ConstructorMode::CreateNewObject)
        return throwError(TypeError, "Illegal constructor", args.GetIsolate());

    return args.This();
}
