static int effect_set_state(struct effect_s *effect, uint32_t state)
{
 int status = 0;
    ALOGV("effect_set_state() id %d, new %d old %d", effect->id, state, effect->state);
 switch(state) {
 case EFFECT_STATE_INIT:
 switch(effect->state) {
 case EFFECT_STATE_ACTIVE:
            session_set_fx_enabled(effect->session, effect->id, false);
 case EFFECT_STATE_CONFIG:
 case EFFECT_STATE_CREATED:
 case EFFECT_STATE_INIT:
 break;
 default:
            BAD_STATE_ABORT(effect->state, state);
 }
 break;
 case EFFECT_STATE_CREATED:
 switch(effect->state) {
 case EFFECT_STATE_INIT:
 break;
 case EFFECT_STATE_CREATED:
 case EFFECT_STATE_ACTIVE:
 case EFFECT_STATE_CONFIG:
            ALOGE("effect_set_state() invalid transition");
            status = -ENOSYS;
 break;
 default:
            BAD_STATE_ABORT(effect->state, state);
 }
 break;
 case EFFECT_STATE_CONFIG:
 switch(effect->state) {
 case EFFECT_STATE_INIT:
            ALOGE("effect_set_state() invalid transition");
            status = -ENOSYS;
 break;
 case EFFECT_STATE_ACTIVE:
            session_set_fx_enabled(effect->session, effect->id, false);
 break;
 case EFFECT_STATE_CREATED:
 case EFFECT_STATE_CONFIG:
 break;
 default:
            BAD_STATE_ABORT(effect->state, state);
 }
 break;
 case EFFECT_STATE_ACTIVE:
 switch(effect->state) {
 case EFFECT_STATE_INIT:
 case EFFECT_STATE_CREATED:
            ALOGE("effect_set_state() invalid transition");
            status = -ENOSYS;
 break;
 case EFFECT_STATE_ACTIVE:
 break;
 case EFFECT_STATE_CONFIG:
            session_set_fx_enabled(effect->session, effect->id, true);
 break;
 default:
            BAD_STATE_ABORT(effect->state, state);
 }
 break;
 default:
        BAD_STATE_ABORT(effect->state, state);
 }

 if (status == 0)
        effect->state = state;

 return status;
}
