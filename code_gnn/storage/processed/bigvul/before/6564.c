Smb4KShare* Smb4KGlobal::findShareByPath( const QString &path )
{
  Smb4KShare *share = NULL;

  mutex.lock();

  if ( !path.isEmpty() && !p->mountedSharesList.isEmpty() )
  {
    for ( int i = 0; i < p->mountedSharesList.size(); ++i )
    {
      if ( QString::compare( path, p->mountedSharesList.at( i )->path(), Qt::CaseInsensitive ) == 0 ||
           QString::compare( path, p->mountedSharesList.at( i )->canonicalPath(), Qt::CaseInsensitive ) == 0 )
      {
        share = p->mountedSharesList.at( i );
        break;
      }
      else
      {
        continue;
      }
    }
  }
  else
  {
  }

  mutex.unlock();

  return share;
}
