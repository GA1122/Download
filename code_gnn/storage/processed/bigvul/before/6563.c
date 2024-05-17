Smb4KShare *Smb4KGlobal::findShare( const QString &name, const QString &host, const QString &workgroup )
{
  Smb4KShare *share = NULL;

  mutex.lock();

  for ( int i = 0; i < p->sharesList.size(); ++i )
  {
    if ( (workgroup.isEmpty() ||
         QString::compare( p->sharesList.at( i )->workgroupName(), workgroup, Qt::CaseInsensitive ) == 0) &&
         QString::compare( p->sharesList.at( i )->hostName(), host, Qt::CaseInsensitive ) == 0 &&
         QString::compare( p->sharesList.at( i )->shareName(), name, Qt::CaseInsensitive ) == 0 )
    {
      share = p->sharesList.at( i );
    }
    else
    {
      continue;
    }
  }

  mutex.unlock();

  return share;
}
