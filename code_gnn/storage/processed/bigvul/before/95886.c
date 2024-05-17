void CL_Reconnect_f( void ) {
	if ( !strlen( cl_reconnectArgs ) )
		return;

	Cbuf_AddText( va("connect %s\n", cl_reconnectArgs ) );
}
