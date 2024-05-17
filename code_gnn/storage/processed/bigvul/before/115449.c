static double propertyValueDouble(JSContextRef context, JSObjectRef object, const char* propertyName)
{
    JSValueRef value = propertyValue(context, object, propertyName);
    if (!value)
        return 0;
    return JSValueToNumber(context, value, 0);    
}
