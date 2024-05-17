v8::Handle<v8::Value> V8DataView::constructorCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.DataView.Constructor");

    if (!args.IsConstructCall())
        return V8Proxy::throwTypeError("DOM object constructor cannot be called as a function");

    if (ConstructorMode::current() == ConstructorMode::WrapExistingObject)
        return args.Holder();

    if (!args.Length()) {
        RefPtr<DataView> dataView = DataView::create(0);
        V8DOMWrapper::setDOMWrapper(args.Holder(), &info, dataView.get());
        V8DOMWrapper::setJSWrapperForDOMObject(dataView.release(), v8::Persistent<v8::Object>::New(args.Holder()));
        return args.Holder();
    }
    if (args[0]->IsNull() || !V8ArrayBuffer::HasInstance(args[0]))
        return V8Proxy::throwTypeError();
    return constructWebGLArrayWithArrayBufferArgument<DataView, char>(args, &info, v8::kExternalByteArray, false);
}
