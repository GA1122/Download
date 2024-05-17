static void Convert_rgn (SDLS_SYNTHESIZER_DATA *pDLSData, EAS_U16 regionIndex, EAS_U16 artIndex, EAS_U16 waveIndex, S_WSMP_DATA *pWsmp)
{
    S_DLS_REGION *pRgn;

  
    pRgn = &pDLSData->pDLS->pDLSRegions[regionIndex];

  
    pRgn->wtRegion.artIndex = artIndex;
    pRgn->wtRegion.waveIndex = waveIndex;

  
  
    pRgn->wtRegion.gain = (EAS_I16) (pWsmp->gain >> 16);
    pRgn->wtRegion.loopStart = pWsmp->loopStart;
    pRgn->wtRegion.loopEnd = (pWsmp->loopStart + pWsmp->loopLength);
    pRgn->wtRegion.tuning = pWsmp->fineTune -(pWsmp->unityNote * 100) + ConvertSampleRate(pWsmp->sampleRate);
 if (pWsmp->loopLength != 0)
        pRgn->wtRegion.region.keyGroupAndFlags |= REGION_FLAG_IS_LOOPED;
}
