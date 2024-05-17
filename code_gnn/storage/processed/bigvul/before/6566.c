Smb4KWorkgroup *Smb4KGlobal::findWorkgroup( const QString &name )
{
  Smb4KWorkgroup *workgroup = NULL;

  mutex.lock();

  for ( int i = 0; i < p->workgroupsList.size(); ++i )
  {
    if ( QString::compare( p->workgroupsList.at( i )->workgroupName(),
         name, Qt::CaseInsensitive ) == 0 )
    {
      workgroup = p->workgroupsList.at( i );
      break;
    }
    else
    {
      continue;
    }
  }

  mutex.unlock();

  return workgroup;
}
