static inline bool isObservable(JSTestCustomNamedGetter* jsTestCustomNamedGetter)
{
    if (jsTestCustomNamedGetter->hasCustomProperties())
        return true;
    return false;
}
