int8_t GASpecificConfig(bitfile *ld, mp4AudioSpecificConfig *mp4ASC,
                        program_config *pce_out)
{
    program_config pce;

     
    mp4ASC->frameLengthFlag = faad_get1bit(ld
        DEBUGVAR(1,138,"GASpecificConfig(): FrameLengthFlag"));
#ifndef ALLOW_SMALL_FRAMELENGTH
    if (mp4ASC->frameLengthFlag == 1)
        return -3;
#endif

    mp4ASC->dependsOnCoreCoder = faad_get1bit(ld
        DEBUGVAR(1,139,"GASpecificConfig(): DependsOnCoreCoder"));
    if (mp4ASC->dependsOnCoreCoder == 1)
    {
        mp4ASC->coreCoderDelay = (uint16_t)faad_getbits(ld, 14
            DEBUGVAR(1,140,"GASpecificConfig(): CoreCoderDelay"));
    }

    mp4ASC->extensionFlag = faad_get1bit(ld DEBUGVAR(1,141,"GASpecificConfig(): ExtensionFlag"));
    if (mp4ASC->channelsConfiguration == 0)
    {
        if (program_config_element(&pce, ld))
            return -3;

        if (pce_out != NULL)
            memcpy(pce_out, &pce, sizeof(program_config));

         
    }

#ifdef ERROR_RESILIENCE
    if (mp4ASC->extensionFlag == 1)
    {
         
        if (mp4ASC->objectTypeIndex >= ER_OBJECT_START)
        {
            mp4ASC->aacSectionDataResilienceFlag = faad_get1bit(ld
                DEBUGVAR(1,144,"GASpecificConfig(): aacSectionDataResilienceFlag"));
            mp4ASC->aacScalefactorDataResilienceFlag = faad_get1bit(ld
                DEBUGVAR(1,145,"GASpecificConfig(): aacScalefactorDataResilienceFlag"));
            mp4ASC->aacSpectralDataResilienceFlag = faad_get1bit(ld
                DEBUGVAR(1,146,"GASpecificConfig(): aacSpectralDataResilienceFlag"));
        }
         
        faad_getbits(ld, 1);
	}
#endif

    return 0;
}
