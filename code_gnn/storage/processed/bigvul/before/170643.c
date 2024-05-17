int Effect_Release(preproc_effect_t *effect)
{
 return Effect_SetState(effect, PREPROC_EFFECT_STATE_INIT);
}
