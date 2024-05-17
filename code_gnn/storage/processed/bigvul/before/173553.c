static int session_release_effect(struct session_s *session,
 struct effect_s *fx)
{
    ALOGW_IF(effect_release(fx) != 0, " session_release_effect() failed for id %d", fx->id);

    session->created_msk &= ~(1<<fx->id);
 if (session->created_msk == 0)
 {
        ALOGV("session_release_effect() last effect: removing session");
        list_remove(&session->node);
        free(session);
 }

 return 0;
}
