static const XMLHttpRequestStaticData* initializeXMLHttpRequestStaticData()
{
    AtomicallyInitializedStatic(const XMLHttpRequestStaticData*, dummy = createXMLHttpRequestStaticData());
    return dummy;
}
