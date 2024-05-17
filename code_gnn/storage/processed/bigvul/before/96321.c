bool CNBL::ParseCSO(TClassPred IsClass, TOffloadPred IsOffload,
                    TSupportedPred IsSupported, LPSTR OffloadName)
{
    ASSERT(IsClass());
    UNREFERENCED_PARAMETER(IsClass);

    if (IsOffload())
    {
        if(!IsSupported())
        {
            DPrintf(0, ("[%s] %s request when it is not supported\n", __FUNCTION__, OffloadName));
#if FAIL_UNEXPECTED
            return false;
#endif
        }
    }
    return true;
}