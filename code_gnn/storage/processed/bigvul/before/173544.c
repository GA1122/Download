static struct session_s *get_session(int32_t id, int32_t  sessionId, int32_t  ioId)
{
 size_t i;
 int free = -1;
 struct listnode *node;
 struct session_s *session;

    list_for_each(node, &session_list) {
        session = node_to_item(node, struct session_s, node);
 if (session->io == ioId) {
 if (session->created_msk & (1 << id)) {
                ALOGV("get_session() effect %d already created", id);
 return NULL;
 }
            ALOGV("get_session() found session %p", session);
 return session;
 }
 }

    session = (struct session_s *)calloc(1, sizeof(struct session_s));
    session_init(session);
    session->id = sessionId;
    session->io = ioId;
    list_add_tail(&session_list, &session->node);

    ALOGV("get_session() created session %p", session);

 return session;
}
