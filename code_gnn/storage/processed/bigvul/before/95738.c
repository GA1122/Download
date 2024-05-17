void CL_UpdateInfoPacket( netadr_t from ) {

	if ( cls.autoupdateServer.type == NA_BAD ) {
		Com_DPrintf( "CL_UpdateInfoPacket:  Auto-Updater has bad address\n" );
		return;
	}

	Com_DPrintf( "Auto-Updater resolved to %i.%i.%i.%i:%i\n",
				 cls.autoupdateServer.ip[0], cls.autoupdateServer.ip[1],
				 cls.autoupdateServer.ip[2], cls.autoupdateServer.ip[3],
				 BigShort( cls.autoupdateServer.port ) );

	if ( !NET_CompareAdr( from, cls.autoupdateServer ) ) {
		Com_DPrintf( "CL_UpdateInfoPacket:  Received packet from %i.%i.%i.%i:%i\n",
					 from.ip[0], from.ip[1], from.ip[2], from.ip[3],
					 BigShort( from.port ) );
		return;
	}

	Cvar_Set( "cl_updateavailable", Cmd_Argv( 1 ) );

	if ( !Q_stricmp( cl_updateavailable->string, "1" ) ) {
		Cvar_Set( "cl_updatefiles", Cmd_Argv( 2 ) );
		VM_Call( uivm, UI_SET_ACTIVE_MENU, UIMENU_WM_AUTOUPDATE );
	}
}
