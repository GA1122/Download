static int effect_create(struct effect_s *effect,
 struct session_s *session,
 effect_handle_t *interface)
{
    effect->session = session;
 *interface = (effect_handle_t)&effect->itfe;
 return effect_set_state(effect, EFFECT_STATE_CREATED);
}
