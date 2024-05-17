static void cmd_parse_myrights(struct ImapData *idata, const char *s)
{
  mutt_debug(2, "Handling MYRIGHTS\n");

  s = imap_next_word((char *) s);
  s = imap_next_word((char *) s);

   
  memset(idata->ctx->rights, 0, sizeof(idata->ctx->rights));

  while (*s && !isspace((unsigned char) *s))
  {
    switch (*s)
    {
      case 'a':
        mutt_bit_set(idata->ctx->rights, MUTT_ACL_ADMIN);
        break;
      case 'e':
        mutt_bit_set(idata->ctx->rights, MUTT_ACL_EXPUNGE);
        break;
      case 'i':
        mutt_bit_set(idata->ctx->rights, MUTT_ACL_INSERT);
        break;
      case 'k':
        mutt_bit_set(idata->ctx->rights, MUTT_ACL_CREATE);
        break;
      case 'l':
        mutt_bit_set(idata->ctx->rights, MUTT_ACL_LOOKUP);
        break;
      case 'p':
        mutt_bit_set(idata->ctx->rights, MUTT_ACL_POST);
        break;
      case 'r':
        mutt_bit_set(idata->ctx->rights, MUTT_ACL_READ);
        break;
      case 's':
        mutt_bit_set(idata->ctx->rights, MUTT_ACL_SEEN);
        break;
      case 't':
        mutt_bit_set(idata->ctx->rights, MUTT_ACL_DELETE);
        break;
      case 'w':
        mutt_bit_set(idata->ctx->rights, MUTT_ACL_WRITE);
        break;
      case 'x':
        mutt_bit_set(idata->ctx->rights, MUTT_ACL_DELMX);
        break;

       
      case 'c':
        mutt_bit_set(idata->ctx->rights, MUTT_ACL_CREATE);
        mutt_bit_set(idata->ctx->rights, MUTT_ACL_DELMX);
        break;
      case 'd':
        mutt_bit_set(idata->ctx->rights, MUTT_ACL_DELETE);
        mutt_bit_set(idata->ctx->rights, MUTT_ACL_EXPUNGE);
        break;
      default:
        mutt_debug(1, "Unknown right: %c\n", *s);
    }
    s++;
  }
}