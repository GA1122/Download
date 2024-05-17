void V8Proxy::setDOMException(int ec, v8::Isolate* isolate)
{
    if (ec <= 0)
        return;

    ExceptionCodeDescription description(ec);

    v8::Handle<v8::Value> exception;
    switch (description.type) {
        DOM_EXCEPTION_INTERFACES_FOR_EACH(TRY_TO_CREATE_EXCEPTION)
    }

    if (exception.IsEmpty())
        return;

    v8::Handle<v8::Value> error = v8::Exception::Error(v8String(description.description, isolate));
    ASSERT(!error.IsEmpty());
    ASSERT(exception->IsObject());
    exception->ToObject()->SetAccessor(v8String("stack", isolate), DOMExceptionStackGetter, DOMExceptionStackSetter, error);

    v8::ThrowException(exception);
}
