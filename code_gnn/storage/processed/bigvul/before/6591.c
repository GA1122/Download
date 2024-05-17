Smb4KMountJob::~Smb4KMountJob()
{
  while ( !m_shares.isEmpty() )
  {
    delete m_shares.takeFirst();
  }

  while ( !m_auth_errors.isEmpty() )
  {
    delete m_auth_errors.takeFirst();
  }

  while ( !m_retries.isEmpty() )
  {
    delete m_retries.takeFirst();
  }
}
