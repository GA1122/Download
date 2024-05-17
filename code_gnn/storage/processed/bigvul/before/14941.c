KillAllClients(void)
{
    int i;

    for (i = 1; i < currentMaxClients; i++)
        if (clients[i]) {
             
            clients[i]->closeDownMode = DestroyAll;
            CloseDownClient(clients[i]);
        }
}
