Smb4KHost *Smb4KGlobal::findHost( const QString &name, const QString &workgroup )
{
  Smb4KHost *host = NULL;

  mutex.lock();

  for ( int i = 0; i < p->hostsList.size(); ++i )
  {
    if ( (workgroup.isEmpty() ||
         QString::compare( p->hostsList.at( i )->workgroupName(),
         workgroup, Qt::CaseInsensitive ) == 0) &&
         QString::compare( p->hostsList.at( i )->hostName(), name,
         Qt::CaseInsensitive ) == 0 )
    {
      host = p->hostsList.at( i );
      break;
    }
    else
    {
      continue;
    }
  }

  mutex.unlock();

  return host;
}
