int64_t ga_get_fd_handle(GAState *s, Error **errp)
{
    int64_t handle;

    g_assert(s->pstate_filepath);
     
    g_assert(!ga_is_frozen(s));

    handle = s->pstate.fd_counter++;

     
    if (s->pstate.fd_counter == INT64_MAX) {
        abort();
    }

    if (!write_persistent_state(&s->pstate, s->pstate_filepath)) {
        error_setg(errp, "failed to commit persistent state to disk");
    }

    return handle;
}
