CStarter::vSSHDFailed(Stream *s,bool retry,char const *fmt,va_list args)
{
	MyString error_msg;
	error_msg.vsprintf( fmt, args );

	error_msg.trim();

	dprintf(D_ALWAYS,"START_SSHD failed: %s\n",error_msg.Value());

	ClassAd response;
	response.Assign(ATTR_RESULT,false);
	response.Assign(ATTR_ERROR_STRING,error_msg);
	if( retry ) {
		response.Assign(ATTR_RETRY,retry);
	}

	s->encode();
	if( !response.put(*s) || !s->end_of_message() ) {
		dprintf(D_ALWAYS,"Failed to send response to START_SSHD.\n");
	}

	return FALSE;
}
