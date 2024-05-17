unsigned long long random_ull(void) {
        int fd;
        uint64_t ull;
        ssize_t r;

        if ((fd = open("/dev/urandom", O_RDONLY|O_CLOEXEC|O_NOCTTY)) < 0)
                goto fallback;

        r = loop_read(fd, &ull, sizeof(ull), true);
        close_nointr_nofail(fd);

        if (r != sizeof(ull))
                goto fallback;

        return ull;

fallback:
        return random() * RAND_MAX + random();
}
