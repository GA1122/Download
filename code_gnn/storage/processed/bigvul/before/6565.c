QList<Smb4KShare *> Smb4KGlobal::findShareByUNC( const QString &unc )
{
  QList<Smb4KShare *> list;

  mutex.lock();

  if ( !unc.isEmpty() && !p->mountedSharesList.isEmpty() )
  {
    for ( int i = 0; i < p->mountedSharesList.size(); ++i )
    {
      if ( QString::compare( unc, p->mountedSharesList.at( i )->unc(), Qt::CaseInsensitive ) == 0 ||
           QString::compare( QString( unc ).replace( ' ', '_' ), p->mountedSharesList.at( i )->unc(), Qt::CaseInsensitive ) == 0  )
      {
        list.append( p->mountedSharesList.at( i ) );
        continue;
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

  return list;
}
