void Smb4KMountJob::setupMount( const QList<Smb4KShare*> &shares, QWidget *parent )
{
  QListIterator<Smb4KShare *> it( shares );

  while ( it.hasNext() )
  {
    Smb4KShare *share = it.next();
    Q_ASSERT( share );
    m_shares << new Smb4KShare( *share );
  }

  m_parent_widget = parent;
}
