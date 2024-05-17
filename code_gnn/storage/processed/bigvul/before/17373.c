PanoramiXResetProc(ExtensionEntry * extEntry)
{
    int i;

    PanoramiXRenderReset();
    PanoramiXFixesReset();
    PanoramiXDamageReset();
#ifdef COMPOSITE
    PanoramiXCompositeReset ();
#endif
    screenInfo.numScreens = PanoramiXNumScreens;
    for (i = 256; i--;)
        ProcVector[i] = SavedProcVector[i];
}
