bool Smb4KGlobal::updateMountedShare(Smb4KShare* share)
{
  Q_ASSERT(share);
  
  bool updated = false;
  
  if (share)
  {
    Smb4KShare *mountedShare = findShareByPath(share->path());
    
    if (mountedShare)
    {
      mountedShare->setMountData(share);
      updated = true;
    }
    else
    {
    }
  }
  else
  {
  }
  
  return updated;
}
