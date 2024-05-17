QList<Smb4KShare *> Smb4KGlobal::sharedResources( Smb4KHost *host )
{
  QList<Smb4KShare *> shares;

  mutex.lock();

  for ( int i = 0; i < p->sharesList.size(); ++i )
  {
    if ( QString::compare( p->sharesList.at( i )->hostName(), host->hostName(), Qt::CaseInsensitive ) == 0 &&
         QString::compare( p->sharesList.at( i )->workgroupName(), host->workgroupName(), Qt::CaseInsensitive ) == 0 )
    {
      shares += p->sharesList.at( i );
      continue;
    }
    else
    {
      continue;
    }
  }

  mutex.unlock();

  return shares;
}
