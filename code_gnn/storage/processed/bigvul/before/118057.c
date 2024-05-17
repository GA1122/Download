v8::Handle<v8::Value> V8XMLHttpRequest::responseTextAccessorGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.XMLHttpRequest.responsetext._get");
    XMLHttpRequest* xmlHttpRequest = V8XMLHttpRequest::toNative(info.Holder());
    ExceptionCode ec = 0;
    const String& text = xmlHttpRequest->responseText(ec);
    if (ec)
        return throwError(ec, info.GetIsolate());
    return v8String(text);
}
