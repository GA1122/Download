static void StreamTcpSessionPoolCleanup(void *s)
{
    if (s != NULL) {
        StreamTcpSessionCleanup(s);
         
        StreamTcpDecrMemuse((uint64_t)sizeof(TcpSession));
    }
}
