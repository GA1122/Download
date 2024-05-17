static QJSValue buildQJSValue(QJSEngine* engine, JSGlobalContextRef context, JSValueRef value, int depth)
{
    QJSValue var;
    JSValueRef exception = 0;

    if (depth > 10)
        return var;

    switch (JSValueGetType(context, value)) {
    case kJSTypeBoolean:
        var = QJSValue(JSValueToBoolean(context, value));
        break;
    case kJSTypeNumber:
        {
            double number = JSValueToNumber(context, value, &exception);
            if (!exception)
                var = QJSValue(number);
        }
        break;
    case kJSTypeString:
        {
            JSRetainPtr<JSStringRef> string = JSValueToStringCopy(context, value, &exception);
            if (!exception)
                var = toQJSValue(string.get());
        }
        break;
    case kJSTypeObject:
        {
            JSObjectRef obj = JSValueToObject(context, value, &exception);

            JSPropertyNameArrayRef names = JSObjectCopyPropertyNames(context, obj);
            size_t length = JSPropertyNameArrayGetCount(names);

            var = engine->newObject();

            for (size_t i = 0; i < length; ++i) {
                JSRetainPtr<JSStringRef> name = JSPropertyNameArrayGetNameAtIndex(names, i);
                JSValueRef property = JSObjectGetProperty(context, obj, name.get(), &exception);

                if (!exception) {
                    QJSValue value = buildQJSValue(engine, context, property, depth + 1);
                    var.setProperty(toQString(name.get()), value);
                }
            }
        }
        break;
    }
    return var;
}
