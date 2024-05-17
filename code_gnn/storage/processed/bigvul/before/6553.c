bool Smb4KGlobal::addMountedShare( Smb4KShare *share )
{
  Q_ASSERT( share );

  bool added = false;

  mutex.lock();

  if ( !findShareByPath( share->path() ) )
  {
    p->mountedSharesList.append( share );
    added = true;

    p->onlyForeignShares = true;

    for ( int i = 0; i < p->mountedSharesList.size(); ++i )
    {
      if ( !p->mountedSharesList.at( i )->isForeign() )
      {
        p->onlyForeignShares = false;
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

  return added;
}
