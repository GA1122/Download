QList<Smb4KShare*> Smb4KGlobal::findInaccessibleShares()
{
  QList<Smb4KShare *> inaccessible_shares;

  mutex.lock();

  for ( int i = 0; i < p->mountedSharesList.size(); ++i )
  {
    if ( p->mountedSharesList.at( i )->isInaccessible() )
    {
      inaccessible_shares.append( p->mountedSharesList.at( i ) );

      continue;
    }
    else
    {
      continue;
    }
  }

  mutex.unlock();

  return inaccessible_shares;
}
