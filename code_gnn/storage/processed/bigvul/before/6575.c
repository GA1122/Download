bool Smb4KGlobal::removeMountedShare( Smb4KShare *share )
{
  Q_ASSERT( share );

  bool removed = false;

  mutex.lock();

  int index = p->mountedSharesList.indexOf( share );

  if ( index != -1 )
  {
    delete p->mountedSharesList.takeAt( index );
    removed = true;

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
    Smb4KShare *s = findShareByPath( share->path() );

    if ( s )
    {
      index = p->mountedSharesList.indexOf( s );

      if ( index != -1 )
      {
        delete p->mountedSharesList.takeAt( index );
        removed = true;

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
    }
    else
    {
    }

    delete share;
  }

  mutex.unlock();

  return removed;
}
