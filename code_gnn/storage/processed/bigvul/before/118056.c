v8::Handle<v8::Value> V8XMLHttpRequest::responseAccessorGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.XMLHttpRequest.response._get");
    XMLHttpRequest* xmlHttpRequest = V8XMLHttpRequest::toNative(info.Holder());

    switch (xmlHttpRequest->responseTypeCode()) {
    case XMLHttpRequest::ResponseTypeDefault:
    case XMLHttpRequest::ResponseTypeText:
        return responseTextAccessorGetter(name, info);

    case XMLHttpRequest::ResponseTypeDocument:
        {
            ExceptionCode ec = 0;
            Document* document = xmlHttpRequest->responseXML(ec);
            if (ec) {
                V8Proxy::setDOMException(ec, info.GetIsolate());
                return v8::Undefined();
            }
            return toV8(document, info.GetIsolate());
        }

    case XMLHttpRequest::ResponseTypeBlob:
#if ENABLE(XHR_RESPONSE_BLOB)
        {
            ExceptionCode ec = 0;
            Blob* blob = xmlHttpRequest->responseBlob(ec);
            if (ec) {
                V8Proxy::setDOMException(ec, info.GetIsolate());
                return v8::Undefined();
            }
            return toV8(blob, info.GetIsolate());
        }
#else
        return v8::Undefined();
#endif

    case XMLHttpRequest::ResponseTypeArrayBuffer:
        {
            ExceptionCode ec = 0;
            ArrayBuffer* arrayBuffer = xmlHttpRequest->responseArrayBuffer(ec);
            if (ec) {
                V8Proxy::setDOMException(ec, info.GetIsolate());
                return v8::Undefined();
            }
            return toV8(arrayBuffer, info.GetIsolate());
        }
    }

    return v8::Undefined();
}
