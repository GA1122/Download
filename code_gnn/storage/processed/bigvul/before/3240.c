s_aes_release(stream_state *ss)
{
    stream_aes_state *const state = (stream_aes_state *) ss;

    if (state->ctx != NULL)
        gs_free_object(state->memory, state->ctx, "aes context structure");
}
