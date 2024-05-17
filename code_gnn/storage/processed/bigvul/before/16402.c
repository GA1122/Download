CStarter::SSHDFailed(Stream *s,char const *fmt,...)
{
	va_list args;
	va_start( args, fmt );
	vSSHDFailed(s,false,fmt,args);
	va_end( args );

	return FALSE;
}
