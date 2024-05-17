JSObject* createRangeError(JSGlobalObject* globalObject, const UString& message)
{
    ASSERT(!message.isEmpty());
    return ErrorInstance::create(globalObject->globalData(), globalObject->rangeErrorConstructor()->errorStructure(), message);
}
