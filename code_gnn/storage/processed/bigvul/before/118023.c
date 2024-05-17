v8::Handle<v8::Value> V8Proxy::throwTypeError(const char* message)
{
     return throwError(TypeError, (message ? message : "Type error"));
 }
