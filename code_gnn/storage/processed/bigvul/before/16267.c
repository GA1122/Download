Condor_Auth_SSL :: ~Condor_Auth_SSL()
{
    ERR_remove_state( 0 );
	if(m_crypto) delete(m_crypto);
}
