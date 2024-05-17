 yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, void *yyscanner, RE_LEX_ENVIRONMENT *lex_env)
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
            case 6:  
#line 96 "re_grammar.y"  
// #line 104 "re_grammar.y"  
        { yr_free(((*yyvaluep).class_vector)); }
#line 1045 "re_grammar.c"  
// #line 1053 "re_grammar.c"  
          break;
  
      case 26:  
#line 97 "re_grammar.y"  
// #line 105 "re_grammar.y"  
        { yr_re_node_destroy(((*yyvaluep).re_node)); }
#line 1051 "re_grammar.c"  
// #line 1059 "re_grammar.c"  
          break;
  
      case 27:  
#line 98 "re_grammar.y"  
// #line 106 "re_grammar.y"  
        { yr_re_node_destroy(((*yyvaluep).re_node)); }
#line 1057 "re_grammar.c"  
// #line 1065 "re_grammar.c"  
          break;
  
      case 28:  
#line 99 "re_grammar.y"  
// #line 107 "re_grammar.y"  
        { yr_re_node_destroy(((*yyvaluep).re_node)); }
#line 1063 "re_grammar.c"  
// #line 1071 "re_grammar.c"  
          break;
  
      case 29:  
#line 100 "re_grammar.y"  
// #line 108 "re_grammar.y"  
        { yr_re_node_destroy(((*yyvaluep).re_node)); }
#line 1069 "re_grammar.c"  
// #line 1077 "re_grammar.c"  
          break;
  
  
       default:
         break;
     }
   YY_IGNORE_MAYBE_UNINITIALIZED_END
 }