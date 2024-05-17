void Smb4KGlobal::initCore(bool modifyCursor, bool initClasses)
{
  p->modifyCursor = modifyCursor;
  
  p->setDefaultSettings();
  
  if (initClasses)
  {
    Smb4KScanner::self()->start();
    Smb4KMounter::self()->start();
  }
  else
  {
  }

  p->makeConnections();
  p->coreInitialized = true;
}
