ProcessCommandLine(int argc, char *argv[])
{
    int i, skip;

    defaultKeyboardControl.autoRepeat = TRUE;

#ifdef NO_PART_NET
    PartialNetwork = FALSE;
#else
    PartialNetwork = TRUE;
#endif

    for ( i = 1; i < argc; i++ )
    {
	 
        if((skip = ddxProcessArgument(argc, argv, i)))
	{
	    i += (skip - 1);
	}
	else if(argv[i][0] ==  ':')  
	{
	     
	    display = argv[i];
	    display++;
            if( ! VerifyDisplayName( display ) ) {
                ErrorF("Bad display name: %s\n", display);
                UseMsg();
		FatalError("Bad display name, exiting: %s\n", display);
            }
	}
	else if ( strcmp( argv[i], "-a") == 0)
	{
	    if(++i < argc)
	        defaultPointerControl.num = atoi(argv[i]);
	    else
		UseMsg();
	}
	else if ( strcmp( argv[i], "-ac") == 0)
	{
	    defeatAccessControl = TRUE;
	}
	else if ( strcmp( argv[i], "-audit") == 0)
	{
	    if(++i < argc)
	        auditTrailLevel = atoi(argv[i]);
	    else
		UseMsg();
	}
	else if ( strcmp( argv[i], "-auth") == 0)
	{
	    if(++i < argc)
	        InitAuthorization (argv[i]);
	    else
		UseMsg();
	}
	else if ( strcmp( argv[i], "-br") == 0) ;  
	else if ( strcmp( argv[i], "+bs") == 0)
	    enableBackingStore = TRUE;
	else if ( strcmp( argv[i], "-bs") == 0)
	    disableBackingStore = TRUE;
	else if ( strcmp( argv[i], "c") == 0)
	{
	    if(++i < argc)
	        defaultKeyboardControl.click = atoi(argv[i]);
	    else
		UseMsg();
	}
	else if ( strcmp( argv[i], "-c") == 0)
	{
	    defaultKeyboardControl.click = 0;
	}
	else if ( strcmp( argv[i], "-cc") == 0)
	{
	    if(++i < argc)
	        defaultColorVisualClass = atoi(argv[i]);
	    else
		UseMsg();
	}
	else if ( strcmp( argv[i], "-core") == 0)
	{
#if !defined(WIN32) || !defined(__MINGW32__)
	    struct rlimit   core_limit;
	    getrlimit (RLIMIT_CORE, &core_limit);
	    core_limit.rlim_cur = core_limit.rlim_max;
	    setrlimit (RLIMIT_CORE, &core_limit);
#endif
	    CoreDump = TRUE;
	}
        else if ( strcmp( argv[i], "-nocursor") == 0)
        {
            EnableCursor = FALSE;
        }
        else if ( strcmp( argv[i], "-dpi") == 0)
	{
	    if(++i < argc)
	        monitorResolution = atoi(argv[i]);
	    else
		UseMsg();
	}
#ifdef DPMSExtension
	else if ( strcmp( argv[i], "dpms") == 0)
	      ;
	else if ( strcmp( argv[i], "-dpms") == 0)
	    DPMSDisabledSwitch = TRUE;
#endif
	else if ( strcmp( argv[i], "-deferglyphs") == 0)
	{
	    if(++i >= argc || !ParseGlyphCachingMode(argv[i]))
		UseMsg();
	}
	else if ( strcmp( argv[i], "-f") == 0)
	{
	    if(++i < argc)
	        defaultKeyboardControl.bell = atoi(argv[i]);
	    else
		UseMsg();
	}
	else if ( strcmp( argv[i], "-fc") == 0)
	{
	    if(++i < argc)
	        defaultCursorFont = argv[i];
	    else
		UseMsg();
	}
	else if ( strcmp( argv[i], "-fn") == 0)
	{
	    if(++i < argc)
	        defaultTextFont = argv[i];
	    else
		UseMsg();
	}
	else if ( strcmp( argv[i], "-fp") == 0)
	{
	    if(++i < argc)
	    {
	        defaultFontPath = argv[i];
	    }
	    else
		UseMsg();
	}
	else if ( strcmp( argv[i], "-help") == 0)
	{
	    UseMsg();
	    exit(0);
	}
        else if ( (skip=XkbProcessArguments(argc,argv,i))!=0 ) {
	    if (skip>0)
		 i+= skip-1;
	    else UseMsg();
	}
#ifdef RLIMIT_DATA
	else if ( strcmp( argv[i], "-ld") == 0)
	{
	    if(++i < argc)
	    {
	        limitDataSpace = atoi(argv[i]);
		if (limitDataSpace > 0)
		    limitDataSpace *= 1024;
	    }
	    else
		UseMsg();
	}
#endif
#ifdef RLIMIT_NOFILE
	else if ( strcmp( argv[i], "-lf") == 0)
	{
	    if(++i < argc)
	        limitNoFile = atoi(argv[i]);
	    else
		UseMsg();
	}
#endif
#ifdef RLIMIT_STACK
	else if ( strcmp( argv[i], "-ls") == 0)
	{
	    if(++i < argc)
	    {
	        limitStackSpace = atoi(argv[i]);
		if (limitStackSpace > 0)
		    limitStackSpace *= 1024;
	    }
	    else
		UseMsg();
	}
#endif
	else if ( strcmp ( argv[i], "-nolock") == 0)
	{
#if !defined(WIN32) && !defined(__CYGWIN__)
	  if (getuid() != 0)
	    ErrorF("Warning: the -nolock option can only be used by root\n");
	  else
#endif
	    nolock = TRUE;
	}
	else if ( strcmp( argv[i], "-nolisten") == 0)
	{
            if(++i < argc) {
		if (_XSERVTransNoListen(argv[i])) 
		    FatalError ("Failed to disable listen for %s transport",
				argv[i]);
	   } else
		UseMsg();
	}
	else if ( strcmp( argv[i], "-noreset") == 0)
	{
	    dispatchExceptionAtReset = 0;
	}
	else if ( strcmp( argv[i], "-reset") == 0)
	{
	    dispatchExceptionAtReset = DE_RESET;
	}
	else if ( strcmp( argv[i], "-p") == 0)
	{
	    if(++i < argc)
	        defaultScreenSaverInterval = ((CARD32)atoi(argv[i])) *
					     MILLI_PER_MIN;
	    else
		UseMsg();
	}
	else if (strcmp(argv[i], "-pogo") == 0)
	{
	    dispatchException = DE_TERMINATE;
	}
	else if ( strcmp( argv[i], "-pn") == 0)
	    PartialNetwork = TRUE;
	else if ( strcmp( argv[i], "-nopn") == 0)
	    PartialNetwork = FALSE;
	else if ( strcmp( argv[i], "r") == 0)
	    defaultKeyboardControl.autoRepeat = TRUE;
	else if ( strcmp( argv[i], "-r") == 0)
	    defaultKeyboardControl.autoRepeat = FALSE;
	else if ( strcmp( argv[i], "-retro") == 0)
	    party_like_its_1989 = TRUE;
	else if ( strcmp( argv[i], "-s") == 0)
	{
	    if(++i < argc)
	        defaultScreenSaverTime = ((CARD32)atoi(argv[i])) *
					 MILLI_PER_MIN;
	    else
		UseMsg();
	}
	else if ( strcmp( argv[i], "-seat") == 0)
	{
	    if(++i < argc)
		SeatId = argv[i];
	    else
		UseMsg();
	}
	else if ( strcmp( argv[i], "-t") == 0)
	{
	    if(++i < argc)
	        defaultPointerControl.threshold = atoi(argv[i]);
	    else
		UseMsg();
	}
	else if ( strcmp( argv[i], "-terminate") == 0)
	{
	    dispatchExceptionAtReset = DE_TERMINATE;
	}
	else if ( strcmp( argv[i], "-to") == 0)
	{
	    if(++i < argc)
		TimeOutValue = ((CARD32)atoi(argv[i])) * MILLI_PER_SECOND;
	    else
		UseMsg();
	}
	else if ( strcmp( argv[i], "-tst") == 0)
	{
	    noTestExtensions = TRUE;
	}
	else if ( strcmp( argv[i], "v") == 0)
	    defaultScreenSaverBlanking = PreferBlanking;
	else if ( strcmp( argv[i], "-v") == 0)
	    defaultScreenSaverBlanking = DontPreferBlanking;
	else if ( strcmp( argv[i], "-wm") == 0)
	    defaultBackingStore = WhenMapped;
        else if ( strcmp( argv[i], "-wr") == 0)
            whiteRoot = TRUE;
        else if ( strcmp( argv[i], "-background") == 0) {
            if(++i < argc) {
                if (!strcmp ( argv[i], "none"))
                    bgNoneRoot = TRUE;
                else
                    UseMsg();
            }
        }
        else if ( strcmp( argv[i], "-maxbigreqsize") == 0) {
             if(++i < argc) {
                 long reqSizeArg = atol(argv[i]);

                  
                 if( reqSizeArg > 0L && reqSizeArg < 128L ) {
                     maxBigRequestSize = (reqSizeArg * 1048576L) - 1L;
                 }
                 else
                 {
                     UseMsg();
                 }
             }
             else
             {
                 UseMsg();
             }
         }
#ifdef PANORAMIX
	else if ( strcmp( argv[i], "+xinerama") == 0){
	    noPanoramiXExtension = FALSE;
	}
	else if ( strcmp( argv[i], "-xinerama") == 0){
	    noPanoramiXExtension = TRUE;
	}
	else if ( strcmp( argv[i], "-disablexineramaextension") == 0){
	    PanoramiXExtensionDisabledHack = TRUE;
	}
#endif
	else if ( strcmp( argv[i], "-I") == 0)
	{
	     
	    break;
	}
	else if (strncmp (argv[i], "tty", 3) == 0)
	{
             
	}
#ifdef XDMCP
	else if ((skip = XdmcpOptions(argc, argv, i)) != i)
	{
	    i = skip - 1;
	}
#endif
	else if ( strcmp( argv[i], "-dumbSched") == 0)
	{
	    SmartScheduleDisable = TRUE;
	}
	else if ( strcmp( argv[i], "-schedInterval") == 0)
	{
	    if (++i < argc)
	    {
		SmartScheduleInterval = atoi(argv[i]);
		SmartScheduleSlice = SmartScheduleInterval;
	    }
	    else
		UseMsg();
	}
	else if ( strcmp( argv[i], "-schedMax") == 0)
	{
	    if (++i < argc)
	    {
		SmartScheduleMaxSlice = atoi(argv[i]);
	    }
	    else
		UseMsg();
	}
	else if ( strcmp( argv[i], "-render" ) == 0)
	{
	    if (++i < argc)
	    {
		int policy = PictureParseCmapPolicy (argv[i]);

		if (policy != PictureCmapPolicyInvalid)
		    PictureCmapPolicy = policy;
		else
		    UseMsg ();
	    }
	    else
		UseMsg ();
	}
	else if ( strcmp( argv[i], "-sigstop") == 0)
	{
	    RunFromSigStopParent = TRUE;
	}
	else if ( strcmp( argv[i], "+extension") == 0)
	{
	    if (++i < argc)
	    {
		if (!EnableDisableExtension(argv[i], TRUE))
		    EnableDisableExtensionError(argv[i], TRUE);
	    }
	    else
		UseMsg();
	}
	else if ( strcmp( argv[i], "-extension") == 0)
	{
	    if (++i < argc)
	    {
		if (!EnableDisableExtension(argv[i], FALSE))
		    EnableDisableExtensionError(argv[i], FALSE);
	    }
	    else
		UseMsg();
	}
 	else
 	{
	    ErrorF("Unrecognized option: %s\n", argv[i]);
	    UseMsg();
	    FatalError("Unrecognized option: %s\n", argv[i]);
        }
    }
}
