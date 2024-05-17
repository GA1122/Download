void MediaPlayerService::removeClient(wp<Client> client)
{
 Mutex::Autolock lock(mLock);

     mClients.remove(client);
 }
