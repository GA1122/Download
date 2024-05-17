FileTransfer::setPeerVersion( const CondorVersionInfo &peer_version )
{
	if ( peer_version.built_since_version(6,7,7) ) {
		TransferFilePermissions = true;
	} else {
		TransferFilePermissions = false;
	}
	if ( peer_version.built_since_version(6,7,19) &&
		 param_boolean( "DELEGATE_JOB_GSI_CREDENTIALS", true ) ) {
		DelegateX509Credentials = true;
	} else {
		DelegateX509Credentials = false;
	}
	if ( peer_version.built_since_version(6,7,20) ) {
		PeerDoesTransferAck = true;
	}
	else {
		PeerDoesTransferAck = false;
		dprintf(D_FULLDEBUG,
			"FileTransfer: peer (version %d.%d.%d) does not support "
			"transfer ack.  Will use older (unreliable) protocol.\n",
			peer_version.getMajorVer(),
			peer_version.getMinorVer(),
			peer_version.getSubMinorVer());
	}
	if( peer_version.built_since_version(6,9,5) ) {
		PeerDoesGoAhead = true;
	}
	else {
		PeerDoesGoAhead = false;
	}

	if( peer_version.built_since_version(7,5,4) ) {
		PeerUnderstandsMkdir = true;
	}
	else {
		PeerUnderstandsMkdir = false;
	}

	if ( peer_version.built_since_version(7,6,0) ) {
		TransferUserLog = false;
	} else {
		TransferUserLog = true;
	}
}
