bool Smb4KGlobal::coreIsRunning()
{
  return (Smb4KScanner::self()->isRunning() ||
          Smb4KMounter::self()->isRunning() ||
          Smb4KPrint::self()->isRunning() ||
          Smb4KSynchronizer::self()->isRunning() ||
          Smb4KPreviewer::self()->isRunning() ||
          Smb4KSearch::self()->isRunning());
}
