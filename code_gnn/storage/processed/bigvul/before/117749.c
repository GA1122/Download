static v8::Handle<v8::Value> supplementalMethod4Callback(const v8::Arguments& args)
{
    INC_STATS("DOM.TestInterface.supplementalMethod4");
    TestSupplemental::supplementalMethod4();
    return v8::Handle<v8::Value>();
}
