void Smb4KMountJob::setupMount( Smb4KShare *share, QWidget *parent )
{
  Q_ASSERT( share );
  m_shares << new Smb4KShare( *share );
  m_parent_widget = parent;
}
