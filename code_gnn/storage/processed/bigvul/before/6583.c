QList<Smb4KHost *> Smb4KGlobal::workgroupMembers( Smb4KWorkgroup *workgroup )
{
  QList<Smb4KHost *> hosts;

  mutex.lock();

  for ( int i = 0; i < p->hostsList.size(); ++i )
  {
    if ( QString::compare( p->hostsList.at( i )->workgroupName(), workgroup->workgroupName(), Qt::CaseInsensitive ) == 0 )
    {
      hosts += p->hostsList.at( i );
      continue;
    }
    else
    {
      continue;
    }
  }

  mutex.unlock();

  return hosts;
}
