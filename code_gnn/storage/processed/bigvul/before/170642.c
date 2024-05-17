int Effect_Init(preproc_effect_t *effect, uint32_t procId)
{
 if (HasReverseStream(procId)) {
        effect->itfe = &sEffectInterfaceReverse;
 } else {
        effect->itfe = &sEffectInterface;
 }
    effect->ops = sPreProcOps[procId];
    effect->procId = procId;
    effect->state = PREPROC_EFFECT_STATE_INIT;
 return 0;
}
