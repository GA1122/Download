PseudoramiXExtensionInit(void)
{
    Bool success = FALSE;
    ExtensionEntry      *extEntry;

    if (noPseudoramiXExtension) return;

    TRACE;

     
#if 0
    if (pseudoramiXNumScreens == 1) {
        noPseudoramiXExtension = TRUE;
        return;
    }
#endif

    if (pseudoramiXGeneration != serverGeneration) {
        extEntry = AddExtension(PANORAMIX_PROTOCOL_NAME, 0, 0,
                                ProcPseudoramiXDispatch,
                                SProcPseudoramiXDispatch,
                                PseudoramiXResetProc,
                                StandardMinorOpcode);
        if (!extEntry) {
            ErrorF("PseudoramiXExtensionInit(): AddExtension failed\n");
        }
        else {
            pseudoramiXGeneration = serverGeneration;
            success = TRUE;
        }
    }

     
    noRRXineramaExtension = success;

    if (!success) {
        ErrorF("%s Extension (PseudoramiX) failed to initialize\n",
               PANORAMIX_PROTOCOL_NAME);
        return;
    }
}
