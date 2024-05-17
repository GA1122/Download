static void reportResourceTiming(ResourceTimingInfo* info, Document* initiatorDocument, bool isMainResource)
{
    if (initiatorDocument && isMainResource)
        initiatorDocument = initiatorDocument->parentDocument();
    if (!initiatorDocument || !initiatorDocument->loader())
        return;
    if (DOMWindow* initiatorWindow = initiatorDocument->domWindow())
        initiatorWindow->performance().addResourceTiming(*info, initiatorDocument);
}
