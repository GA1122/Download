CStarter::SSHDRetry(Stream *s,char const *fmt,...)
{
	va_list args;
	va_start( args, fmt );
	vSSHDFailed(s,true,fmt,args);
	va_end( args );

	return FALSE;
}
