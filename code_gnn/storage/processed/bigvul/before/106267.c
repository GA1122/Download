static inline bool isObservable(JSTestSerializedScriptValueInterface* jsTestSerializedScriptValueInterface)
{
    if (jsTestSerializedScriptValueInterface->hasCustomProperties())
        return true;
    return false;
}
