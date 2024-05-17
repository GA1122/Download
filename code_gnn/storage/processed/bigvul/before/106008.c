static inline bool isObservable(JSTestInterface* jsTestInterface)
{
    if (jsTestInterface->hasCustomProperties())
        return true;
    return false;
}
