void CL_CheckUserinfo( void ) {
	if(clc.state < CA_CONNECTED)
		return;

	if(CL_CheckPaused())
		return;

	if(cvar_modifiedFlags & CVAR_USERINFO)
	{
		cvar_modifiedFlags &= ~CVAR_USERINFO;
		CL_AddReliableCommand(va("userinfo \"%s\"", Cvar_InfoString( CVAR_USERINFO ) ), qfalse);
	}
}
