SafeSock::setTargetSharedPortID( char const *id)
{
	if( id ) {
		dprintf(D_ALWAYS,
			"WARNING: UDP does not support connecting to a shared port! "
			"(requested address is %s with SharedPortID=%s)\n",
			peer_description(), id);
	}
}
