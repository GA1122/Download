s_aes_init(stream_state *ss)
{
    stream_aes_state *const state = (stream_aes_state *) ss;

     
    state->initialized = 0;
    state->ctx = NULL;

    return 0;
}
