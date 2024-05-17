void close_many(const int fds[], unsigned n_fd) {
        unsigned i;

        for (i = 0; i < n_fd; i++)
                close_nointr_nofail(fds[i]);
}
