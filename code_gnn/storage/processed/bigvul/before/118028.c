void V8SVGLength::valueAccessorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.SVGLength.value._set");
    SVGPropertyTearOff<SVGLength>* wrapper = V8SVGLength::toNative(info.Holder());
    if (wrapper->role() == AnimValRole) {
        V8Proxy::setDOMException(NO_MODIFICATION_ALLOWED_ERR, info.GetIsolate());
        return;
    }

    if (!isUndefinedOrNull(value) && !value->IsNumber() && !value->IsBoolean()) {
        V8Proxy::throwTypeError();
        return;
    }

    SVGLength& imp = wrapper->propertyReference();
    ExceptionCode ec = 0;
    SVGLengthContext lengthContext(wrapper->contextElement());
    imp.setValue(static_cast<float>(value->NumberValue()), lengthContext, ec);
    if (UNLIKELY(ec))
        V8Proxy::setDOMException(ec, info.GetIsolate());
    else
        wrapper->commitChange();
}
