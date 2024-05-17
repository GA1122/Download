static void jslLexString() {
  char delim = lex->currCh;
  lex->tokenValue = jsvNewFromEmptyString();
  if (!lex->tokenValue) {
    lex->tk = LEX_EOF;
    return;
  }
  JsvStringIterator it;
  jsvStringIteratorNew(&it, lex->tokenValue, 0);
  jslGetNextCh();
  while (lex->currCh && lex->currCh!=delim) {
    if (lex->currCh == '\\') {
      jslGetNextCh();
      char ch = lex->currCh;
      switch (lex->currCh) {
      case 'n'  : ch = 0x0A; jslGetNextCh(); break;
      case 'b'  : ch = 0x08; jslGetNextCh(); break;
      case 'f'  : ch = 0x0C; jslGetNextCh(); break;
      case 'r'  : ch = 0x0D; jslGetNextCh(); break;
      case 't'  : ch = 0x09; jslGetNextCh(); break;
      case 'v'  : ch = 0x0B; jslGetNextCh(); break;
      case 'u' :
      case 'x' : {  
        char buf[5] = "0x??";
        if (lex->currCh == 'u') {
          jslGetNextCh();
          jslGetNextCh();
        }
        jslGetNextCh();
        buf[2] = lex->currCh; jslGetNextCh();
        buf[3] = lex->currCh; jslGetNextCh();
        ch = (char)stringToInt(buf);
      } break;
      default:
        if (lex->currCh>='0' && lex->currCh<='7') {
          char buf[5] = "0";
          buf[1] = lex->currCh;
          int n=2;
          jslGetNextCh();
          if (lex->currCh>='0' && lex->currCh<='7') {
            buf[n++] = lex->currCh; jslGetNextCh();
            if (lex->currCh>='0' && lex->currCh<='7') {
              buf[n++] = lex->currCh; jslGetNextCh();
            }
          }
          buf[n]=0;
          ch = (char)stringToInt(buf);
        } else {
          jslGetNextCh();
        }
        break;
      }
      jslTokenAppendChar(ch);
      jsvStringIteratorAppend(&it, ch);
    } else if (lex->currCh=='\n' && delim!='`') {
       
      break;
    } else {
      jslTokenAppendChar(lex->currCh);
      jsvStringIteratorAppend(&it, lex->currCh);
      jslGetNextCh();
    }
  }
  jsvStringIteratorFree(&it);
  if (delim=='`')
    lex->tk = LEX_TEMPLATE_LITERAL;
  else lex->tk = LEX_STR;
  if (lex->currCh!=delim)
    lex->tk++;  
  jslGetNextCh();
}
