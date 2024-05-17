void CL_Disconnect_f( void ) {
	SCR_StopCinematic();
	Cvar_Set( "savegame_loading", "0" );
	Cvar_Set( "g_reloading", "0" );
	if ( clc.state != CA_DISCONNECTED && clc.state != CA_CINEMATIC ) {
		Com_Error( ERR_DISCONNECT, "Disconnected from server" );
	}
}
