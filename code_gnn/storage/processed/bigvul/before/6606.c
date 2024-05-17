bool Smb4KGlobal::removeHost(Smb4KHost *host)
{
  Q_ASSERT(host);

  bool removed = false;

  mutex.lock();

  int index = p->hostsList.indexOf(host);

  if (index != -1)
  {
    delete p->hostsList.takeAt(index);
    removed = true;
  }
  else
  {
    Smb4KHost *h = findHost(host->hostName(), host->workgroupName());

    if (h)
    {
      index = p->hostsList.indexOf(h);

      if (index != -1)
      {
        delete p->hostsList.takeAt(index);
        removed = true;
      }
      else
      {
      }
    }
    else
    {
    }

    delete host;
  }

  mutex.unlock();

  return removed;
}
