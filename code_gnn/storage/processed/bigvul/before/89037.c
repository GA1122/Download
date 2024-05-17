WandExport void CLIOption(MagickCLI *cli_wand,const char *option,...)
{
  const char     
    *arg1,
    *arg2;

  CommandOptionFlags
    option_type;

  assert(cli_wand != (MagickCLI *) NULL);
  assert(cli_wand->signature == MagickWandSignature);
  assert(cli_wand->wand.signature == MagickWandSignature);

  do {  

     
    if ( cli_wand->command == (const OptionInfo *) NULL )
      cli_wand->command = GetCommandOptionInfo(option);
#if 0
      (void) FormatLocaleFile(stderr, "CLIOption \"%s\" matched \"%s\"\n",
            option, cli_wand->command->mnemonic );
#endif
    option_type=(CommandOptionFlags) cli_wand->command->flags;

    if ( option_type == UndefinedOptionFlag )
      CLIWandExceptionReturn(OptionFatalError,"UnrecognizedOption",option);

    assert( LocaleCompare(cli_wand->command->mnemonic,option) == 0 );

     
    if ( (option_type & DeprecateOptionFlag) != 0 )
      CLIWandExceptionBreak(OptionError,"DeprecatedOptionNoCode",option);

     
    if ((option_type & (SpecialOptionFlag|GenesisOptionFlag)) != 0 )
      CLIWandExceptionBreak(OptionFatalError,"InvalidUseOfOption",option);

     
    { size_t
        count = (size_t) cli_wand->command->type;

      va_list
        operands;

      va_start(operands,option);

      arg1=arg2=NULL;
      if ( count >= 1 )
        arg1=(const char *) va_arg(operands, const char *);
      if ( count >= 2 )
        arg2=(const char *) va_arg(operands, const char *);

      va_end(operands);
#if 0
      (void) FormatLocaleFile(stderr,
        "CLIOption: \"%s\"  Count: %ld  Flags: %04x  Args: \"%s\" \"%s\"\n",
            option,(long) count,option_type,arg1,arg2);
#endif
    }

     

     
    if ( cli_wand->wand.images != (Image *) NULL )
      (void) SyncImagesSettings(cli_wand->wand.image_info,cli_wand->wand.images,
        cli_wand->wand.exception);

    if ( (option_type & SettingOptionFlags) != 0 ) {
      CLISettingOptionInfo(cli_wand, option, arg1, arg2);
       
    }

     
    if ((option_type & NoImageOperatorFlag) != 0)
      CLINoImageOperator(cli_wand, option, arg1, arg2);

     
    if ( cli_wand->wand.images == (Image *) NULL )
      if ( ((option_type & (SimpleOperatorFlag|ListOperatorFlag)) != 0 ) &&
          ((option_type & SettingOptionFlags) == 0 ))   
        CLIWandExceptionBreak(OptionError,"NoImagesFound",option);

     
    if ( (option_type & SimpleOperatorFlag) != 0 &&
         cli_wand->wand.images != (Image *) NULL)  
      {
        ExceptionInfo *exception=AcquireExceptionInfo();
        (void) CLISimpleOperatorImages(cli_wand, option, arg1, arg2,exception);
        exception=DestroyExceptionInfo(exception);
      }

     
    if ( (option_type & ListOperatorFlag) != 0 )
      (void) CLIListOperatorImages(cli_wand, option, arg1, arg2);

DisableMSCWarning(4127)
  } while (0);   
RestoreMSCWarning

  cli_wand->command = (const OptionInfo *) NULL;  
}
