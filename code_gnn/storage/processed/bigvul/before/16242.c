Condor_Auth_SSL :: Condor_Auth_SSL(ReliSock * sock, int  )
    : Condor_Auth_Base    ( sock, CAUTH_SSL )
{
	m_crypto = NULL;
}
