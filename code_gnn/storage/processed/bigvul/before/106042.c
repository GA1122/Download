static inline bool isObservable(JSTestMediaQueryListListener* jsTestMediaQueryListListener)
{
    if (jsTestMediaQueryListListener->hasCustomProperties())
        return true;
    return false;
}
