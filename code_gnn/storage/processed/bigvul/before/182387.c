  void jslTokenAsString(int token, char *str, size_t len) {
//   assert(len>28);  
    if (token>32 && token<128) {
      assert(len>=4);
     str[0] = '\'';
     str[1] = (char)token;
     str[2] = '\'';
     str[3] = 0;
      return;
    }
  
// 
    switch (token) {
  case LEX_EOF : strncpy(str, "EOF", len); return;
  case LEX_ID : strncpy(str, "ID", len); return;
  case LEX_INT : strncpy(str, "INT", len); return;
  case LEX_FLOAT : strncpy(str, "FLOAT", len); return;
  case LEX_STR : strncpy(str, "STRING", len); return;
  case LEX_UNFINISHED_STR : strncpy(str, "UNFINISHED STRING", len); return;
  case LEX_TEMPLATE_LITERAL : strncpy(str, "TEMPLATE LITERAL", len); return;
  case LEX_UNFINISHED_TEMPLATE_LITERAL : strncpy(str, "UNFINISHED TEMPLATE LITERAL", len); return;
  case LEX_REGEX : strncpy(str, "REGEX", len); return;
  case LEX_UNFINISHED_REGEX : strncpy(str, "UNFINISHED REGEX", len); return;
  case LEX_UNFINISHED_COMMENT : strncpy(str, "UNFINISHED COMMENT", len); return;
//   case LEX_EOF : strcpy(str, "EOF"); return;
//   case LEX_ID : strcpy(str, "ID"); return;
//   case LEX_INT : strcpy(str, "INT"); return;
//   case LEX_FLOAT : strcpy(str, "FLOAT"); return;
//   case LEX_STR : strcpy(str, "STRING"); return;
//   case LEX_UNFINISHED_STR : strcpy(str, "UNFINISHED STRING"); return;
//   case LEX_TEMPLATE_LITERAL : strcpy(str, "TEMPLATE LITERAL"); return;
//   case LEX_UNFINISHED_TEMPLATE_LITERAL : strcpy(str, "UNFINISHED TEMPLATE LITERAL"); return;
//   case LEX_REGEX : strcpy(str, "REGEX"); return;
//   case LEX_UNFINISHED_REGEX : strcpy(str, "UNFINISHED REGEX"); return;
//   case LEX_UNFINISHED_COMMENT : strcpy(str, "UNFINISHED COMMENT"); return;
    }
    if (token>=_LEX_OPERATOR_START && token<_LEX_R_LIST_END) {
      const char tokenNames[] =
           "==\0"
           "===\0"
           "!=\0"
           "!==\0"
           "<=\0"
           "<<\0"
           "<<=\0"
           ">=\0"
           ">>\0"
           ">>>\0"
           ">>=\0"
           ">>>=\0"
           "+=\0"
           "-=\0"
           "++\0"
           "--\0"
           "*=\0"
           "/=\0"
           "%=\0"
           "&=\0"
           "&&\0"
           "|=\0"
           "||\0"
           "^=\0"
           "=>\0"
 
           "if\0"
           "else\0"
           "do\0"
           "while\0"
           "for\0"
           "return\0"
           "continue\0"
           "function\0"
           "return\0"
           "var\0"
           "let\0"
           "const\0"
           "this\0"
           "throw\0"
           "try\0"
           "catch\0"
           "finally\0"
           "true\0"
           "false\0"
           "null\0"
           "undefined\0"
           "new\0"
           "in\0"
           "instanceof\0"
           "switch\0"
           "case\0"
           "default\0"
           "delete\0"
           "typeof\0"
           "void\0"
           "debugger\0"
           "class\0"
           "extends\0"
             "super\0"
           "static\0"
         ;
     unsigned int p = 0;
     int n = token-_LEX_OPERATOR_START;
     while (n>0 && p<sizeof(tokenNames)) {
       while (tokenNames[p] && p<sizeof(tokenNames)) p++;
       p++;  
        n--;  
      }
      assert(n==0);
    strncpy(str, &tokenNames[p], len);
//     strcpy(str, &tokenNames[p]);
      return;
    }
  
  assert(len>=10);
    espruino_snprintf(str, len, "?[%d]", token);
  }