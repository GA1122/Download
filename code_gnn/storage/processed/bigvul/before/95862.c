void Sys_Print( const char *msg )
{
	CON_LogWrite( msg );
	CON_Print( msg );
}
