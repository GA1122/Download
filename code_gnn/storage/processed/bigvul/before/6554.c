bool Smb4KGlobal::addShare( Smb4KShare *share )
{
  Q_ASSERT( share );

  bool added = false;

  mutex.lock();

  if ( !findShare( share->shareName(), share->hostName(), share->workgroupName() ) )
  {
    p->sharesList.append( share );
    added = true;
  }
  else
  {
  }

  mutex.unlock();

  return added;
}
