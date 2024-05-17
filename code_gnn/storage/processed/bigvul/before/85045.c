static void smtp_get_message(char *buffer, char **outptr)
{
  size_t len = strlen(buffer);
  char *message = NULL;

  if(len > 4) {
     
    len -= 4;
    for(message = buffer + 4; *message == ' ' || *message == '\t';
        message++, len--)
      ;

     
    for(; len--;)
      if(message[len] != '\r' && message[len] != '\n' && message[len] != ' ' &&
         message[len] != '\t')
        break;

     
    if(++len) {
      message[len] = '\0';
    }
  }
  else
     
    message = &buffer[len];

  *outptr = message;
}
