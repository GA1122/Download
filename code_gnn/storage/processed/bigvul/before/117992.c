static v8::Local<v8::Value> handleMaxRecursionDepthExceeded()
{
    V8Proxy::throwError(V8Proxy::RangeError, "Maximum call stack size exceeded.");
    return v8::Local<v8::Value>();
}
