yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, void *yyscanner, YR_COMPILER* compiler)
{
  YYUSE (yyvaluep);
  YYUSE (yyscanner);
  YYUSE (compiler);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yytype)
    {
          case 10:  
#line 194 "grammar.y"  
      { yr_free(((*yyvaluep).c_string)); }
#line 1339 "grammar.c"  
        break;

    case 11:  
#line 198 "grammar.y"  
      { yr_free(((*yyvaluep).c_string)); }
#line 1345 "grammar.c"  
        break;

    case 12:  
#line 195 "grammar.y"  
      { yr_free(((*yyvaluep).c_string)); }
#line 1351 "grammar.c"  
        break;

    case 13:  
#line 196 "grammar.y"  
      { yr_free(((*yyvaluep).c_string)); }
#line 1357 "grammar.c"  
        break;

    case 14:  
#line 197 "grammar.y"  
      { yr_free(((*yyvaluep).c_string)); }
#line 1363 "grammar.c"  
        break;

    case 15:  
#line 199 "grammar.y"  
      { yr_free(((*yyvaluep).c_string)); }
#line 1369 "grammar.c"  
        break;

    case 19:  
#line 200 "grammar.y"  
      { yr_free(((*yyvaluep).sized_string)); }
#line 1375 "grammar.c"  
        break;

    case 20:  
#line 201 "grammar.y"  
      { yr_free(((*yyvaluep).sized_string)); }
#line 1381 "grammar.c"  
        break;

    case 21:  
#line 202 "grammar.y"  
      { yr_free(((*yyvaluep).sized_string)); }
#line 1387 "grammar.c"  
        break;


      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}
