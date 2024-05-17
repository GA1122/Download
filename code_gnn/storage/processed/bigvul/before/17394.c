XineramaRegisterConnectionBlockCallback(void (*func) (void))
{
    XineramaConnectionCallbackList *newlist;

    if (!(newlist = malloc(sizeof(XineramaConnectionCallbackList))))
        return FALSE;

    newlist->next = ConnectionCallbackList;
    newlist->func = func;
    ConnectionCallbackList = newlist;

    return TRUE;
}
