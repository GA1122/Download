static int effect_init(struct effect_s *effect, uint32_t id)
{
    effect->itfe = &effect_interface;
    effect->id = id;
    effect->state = EFFECT_STATE_INIT;
 return 0;
}
