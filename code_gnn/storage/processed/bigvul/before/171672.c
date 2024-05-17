static int skt_disconnect(int fd)
{
    INFO("fd %d", fd);

 if (fd != AUDIO_SKT_DISCONNECTED)
 {
        shutdown(fd, SHUT_RDWR);
        close(fd);
 }
 return 0;
}
