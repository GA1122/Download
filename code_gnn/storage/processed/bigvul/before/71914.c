WandExport int ProcessCommandOptions(MagickCLI *cli_wand,int argc,char **argv,
  int index)
{
  const char
    *option,
    *arg1,
    *arg2;

  int
    i,
    end,
    count;

  CommandOptionFlags
    option_type;

  assert(argc>=index);  
  assert(argv != (char **) NULL);
  assert(argv[index] != (char *) NULL);
  assert(argv[argc-1] != (char *) NULL);
  assert(cli_wand != (MagickCLI *) NULL);
  assert(cli_wand->signature == MagickWandSignature);

   
  cli_wand->location="at %s arg %u";
  cli_wand->filename="CLI";
  cli_wand->line=index;   

  if (cli_wand->wand.debug != MagickFalse)
    (void) CLILogEvent(cli_wand,CommandEvent,GetMagickModule(),
         "- Starting (\"%s\")", argv[index]);

  end = argc;
  if ( (cli_wand->process_flags & ProcessImplictWrite) != 0 )
    end--;  

  for (i=index; i < end; i += count +1) {
     
    if ( (cli_wand->process_flags & ProcessOneOptionOnly) != 0 && i != index )
      return(i);

    do {  

      option=argv[i];
      cli_wand->line=i;   

       
      cli_wand->command = GetCommandOptionInfo(argv[i]);
      count=cli_wand->command->type;
      option_type=(CommandOptionFlags) cli_wand->command->flags;
#if 0
      (void) FormatLocaleFile(stderr, "CLI %d: \"%s\" matched \"%s\"\n",
            i, argv[i], cli_wand->command->mnemonic );
#endif

      if ( option_type == UndefinedOptionFlag ||
           (option_type & NonMagickOptionFlag) != 0 ) {
#if MagickCommandDebug >= 3
        (void) FormatLocaleFile(stderr, "CLI arg %d Non-Option: \"%s\"\n",
             i, option);
#endif
        if (IsCommandOption(option) == MagickFalse) {
          if ( (cli_wand->process_flags & ProcessImplictRead) != 0 ) {
             
            cli_wand->command=(const OptionInfo *) NULL;
            CLIOption(cli_wand,"-read",option);
            break;  
          }
        }
        CLIWandException(OptionFatalError,"UnrecognizedOption",option);
        break;  
      }

      if ( ((option_type & SpecialOptionFlag) != 0 ) &&
           ((cli_wand->process_flags & ProcessScriptOption) != 0) &&
           (LocaleCompare(option,"-script") == 0) ) {
         
        if ( (i+count) >= argc )
          CLIWandException(OptionFatalError,"MissingArgument",option);
        ProcessScriptOptions(cli_wand,argv[i+1],argc,argv,i+count);
        return(argc);   
                        
      }

      if ((i+count) >= end ) {
        CLIWandException(OptionFatalError,"MissingArgument",option);
        if ( CLICatchException(cli_wand, MagickFalse) != MagickFalse )
          return(end);
        break;  
      }

      arg1 = ( count >= 1 ) ? argv[i+1] : (char *) NULL;
      arg2 = ( count >= 2 ) ? argv[i+2] : (char *) NULL;

       
#if MagickCommandDebug >= 3
      (void) FormatLocaleFile(stderr,
        "CLI arg %u Option: \"%s\"  Count: %d  Flags: %04x  Args: \"%s\" \"%s\"\n",
            i,option,count,option_type,arg1,arg2);
#endif
       
      if ( (option_type & GenesisOptionFlag) != 0 )
        break;  

       
      if ( (option_type & SpecialOptionFlag) != 0 ) {
        if ( (cli_wand->process_flags & ProcessExitOption) != 0
             && LocaleCompare(option,"-exit") == 0 )
          return(i+count);
        break;  
      }

       
      CLIOption(cli_wand, option, arg1, arg2);

DisableMSCWarning(4127)
    } while (0);  
RestoreMSCWarning

#if MagickCommandDebug >= 5
    (void) FormatLocaleFile(stderr, "CLI-post Image Count = %ld\n",
         (long) GetImageListLength(cli_wand->wand.images) );
#endif
    if ( CLICatchException(cli_wand, MagickFalse) != MagickFalse )
      return(i+count);
  }
  assert(i==end);

  if ( (cli_wand->process_flags & ProcessImplictWrite) == 0 )
    return(end);  

  assert(end==argc-1);  

   
  option=argv[i];
  cli_wand->line=i;

   
  if (cli_wand->image_list_stack != (Stack *) NULL)
    CLIWandException(OptionError,"UnbalancedParenthesis", "(end of cli)");
  else if (cli_wand->image_info_stack != (Stack *) NULL)
    CLIWandException(OptionError,"UnbalancedBraces", "(end of cli)");
  if ( CLICatchException(cli_wand, MagickFalse) != MagickFalse )
    return(argc);

#if MagickCommandDebug >= 3
  (void) FormatLocaleFile(stderr,"CLI arg %d Write File: \"%s\"\n",i,option);
#endif

   
  if (LocaleCompare(option,"-exit") == 0 )
    return(argc);   

   
  if (IsCommandOption(option) != MagickFalse ||
      (option[0] == ' ' && option[1] == '\0') ) {
    CLIWandException(OptionError,"MissingOutputFilename",option);
    return(argc);
  }

  cli_wand->command=(const OptionInfo *) NULL;
  CLIOption(cli_wand,"-write",option);
  return(argc);
}
