void Smb4KGlobal::abortCore()
{
  Smb4KScanner::self()->abortAll();
  Smb4KMounter::self()->abortAll();
  Smb4KPrint::self()->abortAll();
  Smb4KSynchronizer::self()->abortAll();
  Smb4KPreviewer::self()->abortAll();
  Smb4KSearch::self()->abortAll();
}
