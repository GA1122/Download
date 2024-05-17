JSObject* createTypeError(JSGlobalObject* globalObject, const UString& message)
{
    ASSERT(!message.isEmpty());
     return ErrorInstance::create(globalObject->globalData(), globalObject->typeErrorConstructor()->errorStructure(), message);
 }