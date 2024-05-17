 JSObject* createURIError(JSGlobalObject* globalObject, const UString& message)
 {
     ASSERT(!message.isEmpty());
    return ErrorInstance::create(globalObject->globalData(), globalObject->URIErrorConstructor()->errorStructure(), message);
}
