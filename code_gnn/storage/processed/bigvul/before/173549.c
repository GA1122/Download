static int lib_release(effect_handle_t interface)
{
 struct listnode *node;
 struct session_s *session;

    ALOGV("lib_release %p", interface);
 if (init() != 0)
 return init_status;

 struct effect_s *fx = (struct effect_s *)interface;

    list_for_each(node, &session_list) {
        session = node_to_item(node, struct session_s, node);
 if (session == fx->session) {
            session_release_effect(fx->session, fx);
 return 0;
 }
 }

 return -EINVAL;
}
