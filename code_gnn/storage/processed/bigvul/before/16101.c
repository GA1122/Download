void session_free(Session *s) {
        assert(s);

        if (s->in_gc_queue)
                LIST_REMOVE(Session, gc_queue, s->manager->session_gc_queue, s);

        if (s->user) {
                LIST_REMOVE(Session, sessions_by_user, s->user->sessions, s);

                if (s->user->display == s)
                        s->user->display = NULL;
        }

        if (s->seat) {
                if (s->seat->active == s)
                        s->seat->active = NULL;

                LIST_REMOVE(Session, sessions_by_seat, s->seat->sessions, s);
        }

        if (s->cgroup_path)
                hashmap_remove(s->manager->cgroups, s->cgroup_path);

        free(s->cgroup_path);
        strv_free(s->controllers);

        free(s->tty);
        free(s->display);
        free(s->remote_host);
        free(s->remote_user);
        free(s->service);

        hashmap_remove(s->manager->sessions, s->id);

        session_remove_fifo(s);

        free(s->state_file);
        free(s);
}
