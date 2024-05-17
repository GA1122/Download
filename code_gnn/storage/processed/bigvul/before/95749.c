static int CL_WalkDemoExt(char *arg, char *name, int *demofile)
{
	int i = 0;
	*demofile = 0;

#ifdef LEGACY_PROTOCOL
	if(com_legacyprotocol->integer > 0)
	{
		Com_sprintf(name, MAX_OSPATH, "demos/%s.%s%d", arg, DEMOEXT, com_legacyprotocol->integer);
		FS_FOpenFileRead(name, demofile, qtrue);
		
		if (*demofile)
		{
			Com_Printf("Demo file: %s\n", name);
			return com_legacyprotocol->integer;
		}
	}
	
	if(com_protocol->integer != com_legacyprotocol->integer)
#endif
	{
		Com_sprintf(name, MAX_OSPATH, "demos/%s.%s%d", arg, DEMOEXT, com_protocol->integer);
		FS_FOpenFileRead(name, demofile, qtrue);

		if (*demofile)
		{
			Com_Printf("Demo file: %s\n", name);
			return com_protocol->integer;
		}
	}

	Com_Printf("Not found: %s\n", name);

	while(demo_protocols[i])
	{
#ifdef LEGACY_PROTOCOL
		if(demo_protocols[i] == com_legacyprotocol->integer)
			continue;
#endif
		if(demo_protocols[i] == com_protocol->integer)
			continue;
	
		Com_sprintf (name, MAX_OSPATH, "demos/%s.%s%d", arg, DEMOEXT, demo_protocols[i]);
		FS_FOpenFileRead( name, demofile, qtrue );
		if (*demofile)
		{
			Com_Printf("Demo file: %s\n", name);

			return demo_protocols[i];
		}
		else
			Com_Printf("Not found: %s\n", name);
		i++;
	}
	
	return -1;
}
