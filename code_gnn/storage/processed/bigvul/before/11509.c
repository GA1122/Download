void close_nointr_nofail(int fd) {
        int saved_errno = errno;

         

        assert_se(close_nointr(fd) == 0);

        errno = saved_errno;
}
