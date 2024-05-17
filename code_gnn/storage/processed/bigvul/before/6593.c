bool Smb4KGlobal::addHost(Smb4KHost *host)
{
  Q_ASSERT(host);

  bool added = false;

  mutex.lock();

  if (!findHost(host->hostName(), host->workgroupName()))
  {
    p->hostsList.append(host);
    added = true;
  }
  else
  {
  }

  mutex.unlock();

  return added;
}
