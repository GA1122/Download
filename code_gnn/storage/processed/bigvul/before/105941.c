static inline bool isObservable(JSTestActiveDOMObject* jsTestActiveDOMObject)
{
    if (jsTestActiveDOMObject->hasCustomProperties())
        return true;
    return false;
}
