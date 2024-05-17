 WandExport MagickBooleanType MagickImageCommand(ImageInfo *image_info,int argc,
  char **argv,char **metadata,ExceptionInfo *exception)
{
  MagickCLI
    *cli_wand;

  size_t
    len;

  assert(image_info != (ImageInfo *) NULL);

   
  ReadCommandlLine(argc,&argv);

   
  cli_wand=AcquireMagickCLI(image_info,exception);
  cli_wand->location="Initializing";
  cli_wand->filename=argv[0];
  cli_wand->line=1;

  if (cli_wand->wand.debug != MagickFalse)
    (void) CLILogEvent(cli_wand,CommandEvent,GetMagickModule(),
         "\"%s\"",argv[0]);


  GetPathComponent(argv[0],TailPath,cli_wand->wand.name);
  SetClientName(cli_wand->wand.name);
  ConcatenateMagickString(cli_wand->wand.name,"-CLI",MagickPathExtent);

  len=strlen(argv[0]);   

   
  if (len>=7 && LocaleCompare("convert",argv[0]+len-7) == 0) {
    cli_wand->process_flags = ConvertCommandOptionFlags;
    (void) FormatLocaleFile(stderr,"WARNING: %s\n",
         "The convert command is deprecated in IMv7, use \"magick\"\n");
  }

   
  if (len>=6 && LocaleCompare("script",argv[0]+len-6) == 0) {
    if (argc >= 2 && (  (*(argv[1]) != '-') || (strlen(argv[1]) == 1) )) {
      GetPathComponent(argv[1],TailPath,cli_wand->wand.name);
      ProcessScriptOptions(cli_wand,argv[1],argc,argv,2);
      goto Magick_Command_Cleanup;
    }
  }

   
  if (argc == 2) {
    if ((LocaleCompare("-version",argv[1]) == 0)   ||  
        (LocaleCompare("--version",argv[1]) == 0) ) {  
      CLIOption(cli_wand, "-version");
      goto Magick_Command_Exit;
    }
    if ((LocaleCompare("-help",argv[1]) == 0)   ||  
        (LocaleCompare("--help",argv[1]) == 0) ) {  
      if (cli_wand->wand.debug != MagickFalse)
        (void) CLILogEvent(cli_wand,CommandEvent,GetMagickModule(),
            "- Special Option \"%s\"", argv[1]);
      MagickUsage(MagickFalse);
      goto Magick_Command_Exit;
    }
    if (LocaleCompare("-usage",argv[1]) == 0) {    
      if (cli_wand->wand.debug != MagickFalse)
        (void) CLILogEvent(cli_wand,CommandEvent,GetMagickModule(),
            "- Special Option \"%s\"", argv[1]);
      CLIOption(cli_wand, "-version" );
      MagickUsage(MagickTrue);
      goto Magick_Command_Exit;
    }
  }

   
  if (argc < 3) {
    (void) FormatLocaleFile(stderr,
       "Error: Invalid argument or not enough arguments\n\n");
    MagickUsage(MagickFalse);
    goto Magick_Command_Exit;
  }

   
  if (LocaleCompare("-concatenate",argv[1]) == 0) {
    if (cli_wand->wand.debug != MagickFalse)
        (void) CLILogEvent(cli_wand,CommandEvent,GetMagickModule(),
            "- Special Option \"%s\"", argv[1]);
    ConcatenateImages(argc,argv,exception);
    goto Magick_Command_Exit;
  }

   
  if (argc == 3 && LocaleCompare("-list",argv[1]) == 0) {
    CLIOption(cli_wand, argv[1], argv[2]);
    goto Magick_Command_Exit;
  }

   
   

  if (LocaleCompare("-script",argv[1]) == 0) {
     
    GetPathComponent(argv[2],TailPath,cli_wand->wand.name);
    ProcessScriptOptions(cli_wand,argv[2],argc,argv,3);
  }
  else {
     
    ProcessCommandOptions(cli_wand,argc,argv,1);
  }
   

Magick_Command_Cleanup:
  cli_wand->location="Cleanup";
  cli_wand->filename=argv[0];
  if (cli_wand->wand.debug != MagickFalse)
    (void) CLILogEvent(cli_wand,CommandEvent,GetMagickModule(),
         "\"%s\"",argv[0]);

   
  while ((cli_wand->image_list_stack != (Stack *) NULL) &&
         (cli_wand->image_list_stack->next != (Stack *) NULL))
    CLIOption(cli_wand,")");
  while ((cli_wand->image_info_stack != (Stack *) NULL) &&
         (cli_wand->image_info_stack->next != (Stack *) NULL))
    CLIOption(cli_wand,"}");
