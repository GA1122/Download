static int propertyValueInt(JSContextRef context, JSObjectRef object, const char* propertyName)
{
    return static_cast<int>(propertyValueDouble(context, object, propertyName));    
}
