void CL_Reconnect_f( void ) {
	if ( !strlen( cl_reconnectArgs ) )
		return;
	Cvar_Set("ui_singlePlayerActive", "0");
	Cbuf_AddText( va("connect %s\n", cl_reconnectArgs ) );
}