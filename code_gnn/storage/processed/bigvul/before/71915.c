WandExport void ProcessScriptOptions(MagickCLI *cli_wand,const char *filename,
  int argc,char **argv,int index)
{
  ScriptTokenInfo
    *token_info;

  CommandOptionFlags
    option_type;

  int
    count;

  char
    *option,
    *arg1,
    *arg2;

  assert(filename != (char *) NULL );  
  assert(cli_wand != (MagickCLI *) NULL);
  assert(cli_wand->signature == MagickWandSignature);
  if (cli_wand->wand.debug != MagickFalse)
    (void) LogMagickEvent(CommandEvent,GetMagickModule(),
         "Processing script \"%s\"", filename);

   
  token_info = AcquireScriptTokenInfo(filename);
  if (token_info == (ScriptTokenInfo *) NULL) {
    CLIWandExceptionFile(OptionFatalError,"UnableToOpenScript",filename);
    return;
  }

   
  cli_wand->location="in \"%s\" at line %u,column %u";
  if ( LocaleCompare("-", filename) == 0 )
    cli_wand->filename="stdin";
  else
    cli_wand->filename=filename;

   
  option = arg1 = arg2 = (char*) NULL;
DisableMSCWarning(4127)
  while (1) {
RestoreMSCWarning

    { MagickBooleanType status = GetScriptToken(token_info);
      cli_wand->line=token_info->token_line;
      cli_wand->column=token_info->token_column;
      if (status == MagickFalse)
        break;  
    }

    do {  

       
      CloneString(&option,token_info->token);

       
      cli_wand->command = GetCommandOptionInfo(option);
      count=cli_wand->command->type;
      option_type=(CommandOptionFlags) cli_wand->command->flags;
#if 0
      (void) FormatLocaleFile(stderr, "Script: %u,%u: \"%s\" matched \"%s\"\n",
          cli_wand->line, cli_wand->line, option, cli_wand->command->mnemonic );
#endif

       
      if ( option_type == UndefinedOptionFlag ||
           (option_type & NonMagickOptionFlag) != 0 ) {
#if MagickCommandDebug >= 3
        (void) FormatLocaleFile(stderr, "Script %u,%u Non-Option: \"%s\"\n",
                    cli_wand->line, cli_wand->line, option);
#endif
        if (IsCommandOption(option) == MagickFalse) {
           
          cli_wand->command=(const OptionInfo *) NULL;
          CLIOption(cli_wand,"-read",option);
          break;  
        }
        CLIWandException(OptionFatalError,"UnrecognizedOption",option);
        break;  
      }

      if ( count >= 1 ) {
        if (GetScriptToken(token_info) == MagickFalse)
          CLIWandException(OptionFatalError,"MissingArgument",option);
        CloneString(&arg1,token_info->token);
      }
      else
        CloneString(&arg1,(char *) NULL);

      if ( count >= 2 ) {
        if (GetScriptToken(token_info) == MagickFalse)
          CLIWandExceptionBreak(OptionFatalError,"MissingArgument",option);
        CloneString(&arg2,token_info->token);
      }
      else
        CloneString(&arg2,(char *) NULL);

       
#if MagickCommandDebug >= 3
      (void) FormatLocaleFile(stderr,
        "Script %u,%u Option: \"%s\"  Count: %d  Flags: %04x  Args: \"%s\" \"%s\"\n",
            cli_wand->line,cli_wand->line,option,count,option_type,arg1,arg2);
#endif
       
      if ( (option_type & DeprecateOptionFlag) != 0 ) {
        CLIWandException(OptionError,"DeprecatedOptionNoCode",option);
        break;  
      }

       
      if ( (option_type & GenesisOptionFlag) != 0 ) {
        CLIWandException(OptionError,"InvalidUseOfOption",option);
        break;  
      }

       
      if ( (option_type & SpecialOptionFlag) != 0 ) {
        if ( LocaleCompare(option,"-exit") == 0 ) {
          goto loop_exit;  
        }
        if ( LocaleCompare(option,"-script") == 0 ) {
           
          CLIWandException(OptionError,"InvalidUseOfOption",option);
          break;  
        }
         
         
        CLIWandException(OptionError,"InvalidUseOfOption",option);
        break;  
      }

       
      CLIOption(cli_wand, option, arg1, arg2);
      (void) fflush(stdout);
      (void) fflush(stderr);

DisableMSCWarning(4127)
    } while (0);  
RestoreMSCWarning

#if MagickCommandDebug >= 5
    fprintf(stderr, "Script Image Count = %ld\n",
         GetImageListLength(cli_wand->wand.images) );
#endif
    if (CLICatchException(cli_wand, MagickFalse) != MagickFalse)
      break;   
  }

   
loop_exit:
#if MagickCommandDebug >= 3
  (void) FormatLocaleFile(stderr, "Script End: %d\n", token_info->status);
#endif
  switch( token_info->status ) {
    case TokenStatusOK:
    case TokenStatusEOF:
      if (cli_wand->image_list_stack != (Stack *) NULL)
        CLIWandException(OptionError,"UnbalancedParenthesis", "(eof)");
      else if (cli_wand->image_info_stack != (Stack *) NULL)
        CLIWandException(OptionError,"UnbalancedBraces", "(eof)");
      break;
    case TokenStatusBadQuotes:
       
      if( strlen(token_info->token) > INITAL_TOKEN_LENGTH-1 ) {
        token_info->token[INITAL_TOKEN_LENGTH-4] = '.';
        token_info->token[INITAL_TOKEN_LENGTH-3] = '.';
        token_info->token[INITAL_TOKEN_LENGTH-2] = '.';
        token_info->token[INITAL_TOKEN_LENGTH-1] = '\0';
      }
      CLIWandException(OptionFatalError,"ScriptUnbalancedQuotes",
           token_info->token);
      break;
    case TokenStatusMemoryFailed:
      CLIWandException(OptionFatalError,"ScriptTokenMemoryFailed","");
      break;
    case TokenStatusBinary:
      CLIWandException(OptionFatalError,"ScriptIsBinary","");
      break;
  }
  (void) fflush(stdout);
  (void) fflush(stderr);
  if (cli_wand->wand.debug != MagickFalse)
    (void) LogMagickEvent(CommandEvent,GetMagickModule(),
         "Script End \"%s\"", filename);

   
  token_info = DestroyScriptTokenInfo(token_info);

  CloneString(&option,(char *) NULL);
  CloneString(&arg1,(char *) NULL);
  CloneString(&arg2,(char *) NULL);

  return;
}
