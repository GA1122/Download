static v8::Handle<v8::Value> classMethodCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestObj.classMethod");
    TestObj::classMethod();
    return v8::Handle<v8::Value>();
}
