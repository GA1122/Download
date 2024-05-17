int Reverb_LoadPreset(ReverbContext *pContext)
{
    pContext->curPreset = pContext->nextPreset;

 if (pContext->curPreset != REVERB_PRESET_NONE) {
 const t_reverb_settings *preset = &sReverbPresets[pContext->curPreset];
 ReverbSetRoomLevel(pContext, preset->roomLevel);
 ReverbSetRoomHfLevel(pContext, preset->roomHFLevel);
 ReverbSetDecayTime(pContext, preset->decayTime);
 ReverbSetDecayHfRatio(pContext, preset->decayHFRatio);
 ReverbSetReverbLevel(pContext, preset->reverbLevel);
 ReverbSetDiffusion(pContext, preset->diffusion);
 ReverbSetDensity(pContext, preset->density);
 }

 return 0;
}
