v8::Handle<v8::Value> V8SVGLength::valueAccessorGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    INC_STATS("DOM.SVGLength.value._get");
    SVGPropertyTearOff<SVGLength>* wrapper = V8SVGLength::toNative(info.Holder());
    SVGLength& imp = wrapper->propertyReference();
    ExceptionCode ec = 0;
    SVGLengthContext lengthContext(wrapper->contextElement());
    float value = imp.value(lengthContext, ec);
    if (UNLIKELY(ec)) {
        V8Proxy::setDOMException(ec, info.GetIsolate());
        return v8::Handle<v8::Value>();
    }
    return v8::Number::New(value);
}
