char * SafeSock::serialize() const
{

	char * parent_state = Sock::serialize();
	char outbuf[50];

    memset(outbuf, 0, 50);

	sprintf(outbuf,"%d*%s*", _special_state, _who.to_sinful().Value());
	strcat(parent_state,outbuf);

	return( parent_state );
}
