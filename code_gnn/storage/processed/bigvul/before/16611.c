FileTransfer::setPeerVersion( const char *peer_version )
{
	CondorVersionInfo vi( peer_version );

	setPeerVersion( vi );
}
