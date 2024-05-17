 yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, void *yyscanner, HEX_LEX_ENVIRONMENT *lex_env)
 {
   YYUSE (yyvaluep);
   YYUSE (yyscanner);
   YYUSE (lex_env);
   if (!yymsg)
     yymsg = "Deleting";
   YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);
 
   YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
    switch (yytype)
      {
            case 16:  
#line 94 "hex_grammar.y"  
// #line 101 "hex_grammar.y"  
        { yr_re_node_destroy(((*yyvaluep).re_node)); }
#line 1023 "hex_grammar.c"  
// #line 1030 "hex_grammar.c"  
          break;
  
      case 17:  
#line 95 "hex_grammar.y"  
// #line 102 "hex_grammar.y"  
        { yr_re_node_destroy(((*yyvaluep).re_node)); }
#line 1029 "hex_grammar.c"  
// #line 1036 "hex_grammar.c"  
          break;
  
      case 18:  
#line 96 "hex_grammar.y"  
// #line 103 "hex_grammar.y"  
        { yr_re_node_destroy(((*yyvaluep).re_node)); }
#line 1035 "hex_grammar.c"  
// #line 1042 "hex_grammar.c"  
          break;
  
      case 19:  
#line 97 "hex_grammar.y"  
// #line 104 "hex_grammar.y"  
        { yr_re_node_destroy(((*yyvaluep).re_node)); }
#line 1041 "hex_grammar.c"  
// #line 1048 "hex_grammar.c"  
          break;
  
      case 21:  
#line 100 "hex_grammar.y"  
// #line 107 "hex_grammar.y"  
        { yr_re_node_destroy(((*yyvaluep).re_node)); }
#line 1047 "hex_grammar.c"  
// #line 1054 "hex_grammar.c"  
          break;
  
      case 22:  
#line 99 "hex_grammar.y"  
// #line 106 "hex_grammar.y"  
        { yr_re_node_destroy(((*yyvaluep).re_node)); }
#line 1053 "hex_grammar.c"  
// #line 1060 "hex_grammar.c"  
          break;
  
      case 23:  
#line 98 "hex_grammar.y"  
// #line 105 "hex_grammar.y"  
        { yr_re_node_destroy(((*yyvaluep).re_node)); }
#line 1059 "hex_grammar.c"  
// #line 1066 "hex_grammar.c"  
          break;
  
  
       default:
         break;
     }
   YY_IGNORE_MAYBE_UNINITIALIZED_END
 }