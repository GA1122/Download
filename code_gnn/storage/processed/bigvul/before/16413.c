CStarter::classadCommand( int, Stream* s )
{
	ClassAd ad;
	ReliSock* rsock = (ReliSock*)s;
	int cmd = 0;

	cmd = getCmdFromReliSock( rsock, &ad, false );

	switch( cmd ) {
	case FALSE:
		return FALSE;
		break;

	case CA_RECONNECT_JOB:
		return jic->reconnect( rsock, &ad );
		break;

	default:
		const char* tmp = getCommandString(cmd);
		MyString err_msg = "Starter does not support command (";
		err_msg += tmp;
		err_msg += ')';
		sendErrorReply( s, tmp, CA_INVALID_REQUEST, err_msg.Value() );
		return FALSE;
	}
	return TRUE;
}
