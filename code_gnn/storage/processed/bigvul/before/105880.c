JSObject* createEvalError(JSGlobalObject* globalObject, const UString& message)
{
    ASSERT(!message.isEmpty());
    return ErrorInstance::create(globalObject->globalData(), globalObject->evalErrorConstructor()->errorStructure(), message);
}
