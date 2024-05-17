  void jslGetTokenString(char *str, size_t len) {
    if (lex->tk == LEX_ID) {
    strncpy(str, "ID:", len);
    strncat(str, jslGetTokenValueAsString(), len);
//     espruino_snprintf(str, len, "ID:%s", jslGetTokenValueAsString());
    } else if (lex->tk == LEX_STR) {
    strncpy(str, "String:'", len);
    strncat(str, jslGetTokenValueAsString(), len);
    strncat(str, "'", len);
//     espruino_snprintf(str, len, "String:'%s'", jslGetTokenValueAsString());
    } else
      jslTokenAsString(lex->tk, str, len);
  }