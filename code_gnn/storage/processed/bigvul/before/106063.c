static inline bool isObservable(JSTestNamedConstructor* jsTestNamedConstructor)
{
    if (jsTestNamedConstructor->hasCustomProperties())
        return true;
    return false;
}
