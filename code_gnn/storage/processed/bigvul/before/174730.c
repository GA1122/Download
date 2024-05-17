static void dump_list_l()
{
 struct listnode *node;
 vol_listener_context_t *context;

    ALOGW("DUMP_START :: ===========");

    list_for_each(node, &vol_effect_list) {
        context = node_to_item(node, struct vol_listener_context_s, effect_list_node);
        ALOGW("%s: streamType [%s] Device [%d] state [%d] sessionID [%d] volume (L/R) [%f / %f] ",
                __func__,
                context->stream_type == MUSIC ? "MUSIC" :
                context->stream_type == RING ? "RING" :
                context->stream_type == ALARM ? "ALARM" :
                context->stream_type == VOICE_CALL ? "VOICE_CALL" :
                context->stream_type == NOTIFICATION ? "NOTIFICATION" : "--INVALID--",
                context->dev_id, context->state, context->session_id, context->left_vol,context->right_vol);
 }

    ALOGW("DUMP_END :: ===========");
}
