void CL_GlobalServers_f( void ) {
	netadr_t	to;
	int			count, i, masterNum;
	char		command[1024], *masteraddress;
	
	if ((count = Cmd_Argc()) < 3 || (masterNum = atoi(Cmd_Argv(1))) < 0 || masterNum > MAX_MASTER_SERVERS - 1)
	{
		Com_Printf("usage: globalservers <master# 0-%d> <protocol> [keywords]\n", MAX_MASTER_SERVERS - 1);
		return;	
	}

	sprintf(command, "sv_master%d", masterNum + 1);
	masteraddress = Cvar_VariableString(command);
	
	if(!*masteraddress)
	{
		Com_Printf( "CL_GlobalServers_f: Error: No master server address given.\n");
		return;	
	}


	i = NET_StringToAdr(masteraddress, &to, NA_UNSPEC);
	
	if(!i)
	{
		Com_Printf( "CL_GlobalServers_f: Error: could not resolve address of master %s\n", masteraddress);
		return;	
	}
	else if(i == 2)
		to.port = BigShort(PORT_MASTER);

	Com_Printf("Requesting servers from master %s...\n", masteraddress);

	cls.numglobalservers = -1;
	cls.pingUpdateSource = AS_GLOBAL;

	if (to.type == NA_IP6 || to.type == NA_MULTICAST6)
	{
		int v4enabled = Cvar_VariableIntegerValue("net_enabled") & NET_ENABLEV4;
		
		if(v4enabled)
		{
			Com_sprintf(command, sizeof(command), "getserversExt %s %s",
				com_gamename->string, Cmd_Argv(2));
		}
		else
		{
			Com_sprintf(command, sizeof(command), "getserversExt %s %s ipv6",
				com_gamename->string, Cmd_Argv(2));
		}
	}
	else if ( !Q_stricmp( com_gamename->string, LEGACY_MASTER_GAMENAME ) )
		Com_sprintf(command, sizeof(command), "getservers %s",
			Cmd_Argv(2));
	else
		Com_sprintf(command, sizeof(command), "getservers %s %s",
			com_gamename->string, Cmd_Argv(2));

	for (i=3; i < count; i++)
	{
		Q_strcat(command, sizeof(command), " ");
		Q_strcat(command, sizeof(command), Cmd_Argv(i));
	}

	NET_OutOfBandPrint( NS_SERVER, to, "%s", command );
}
