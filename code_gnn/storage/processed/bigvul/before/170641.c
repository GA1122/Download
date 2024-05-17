int Effect_Create(preproc_effect_t *effect,
 preproc_session_t *session,
 effect_handle_t *interface)
{
    effect->session = session;
 *interface = (effect_handle_t)&effect->itfe;
 return Effect_SetState(effect, PREPROC_EFFECT_STATE_CREATED);
}
