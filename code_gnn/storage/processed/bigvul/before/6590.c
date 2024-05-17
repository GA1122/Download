void Smb4KMountJob::start()
{
  m_started = true;
  QTimer::singleShot( 50, this, SLOT(slotStartMount()) );
}
