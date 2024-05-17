 yyparse (void *yyscanner, RE_LEX_ENVIRONMENT *lex_env)
 {
  
 int yychar;
 
 
  
  
 YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
 YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);
 
      
     int yynerrs;
 
     int yystate;
      
     int yyerrstatus;
 
      
 
      
     yytype_int16 yyssa[YYINITDEPTH];
     yytype_int16 *yyss;
     yytype_int16 *yyssp;
 
      
     YYSTYPE yyvsa[YYINITDEPTH];
     YYSTYPE *yyvs;
     YYSTYPE *yyvsp;
 
     YYSIZE_T yystacksize;
 
   int yyn;
   int yyresult;
    
   int yytoken = 0;
    
   YYSTYPE yyval;
 
 #if YYERROR_VERBOSE
    
   char yymsgbuf[128];
   char *yymsg = yymsgbuf;
   YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
 #endif
 
 #define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))
 
    
   int yylen = 0;
 
   yyssp = yyss = yyssa;
   yyvsp = yyvs = yyvsa;
   yystacksize = YYINITDEPTH;
 
   YYDPRINTF ((stderr, "Starting parse\n"));
 
   yystate = 0;
   yyerrstatus = 0;
   yynerrs = 0;
   yychar = YYEMPTY;  
   goto yysetstate;
 
  
  yynewstate:
    
   yyssp++;
 
  yysetstate:
   *yyssp = yystate;
 
   if (yyss + yystacksize - 1 <= yyssp)
     {
        
       YYSIZE_T yysize = yyssp - yyss + 1;
 
 #ifdef yyoverflow
       {
          
         YYSTYPE *yyvs1 = yyvs;
         yytype_int16 *yyss1 = yyss;
 
          
         yyoverflow (YY_("memory exhausted"),
                     &yyss1, yysize * sizeof (*yyssp),
                     &yyvs1, yysize * sizeof (*yyvsp),
                     &yystacksize);
 
         yyss = yyss1;
         yyvs = yyvs1;
       }
 #else  
 # ifndef YYSTACK_RELOCATE
       goto yyexhaustedlab;
 # else
        
       if (YYMAXDEPTH <= yystacksize)
         goto yyexhaustedlab;
       yystacksize *= 2;
       if (YYMAXDEPTH < yystacksize)
         yystacksize = YYMAXDEPTH;
 
       {
         yytype_int16 *yyss1 = yyss;
         union yyalloc *yyptr =
           (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
         if (! yyptr)
           goto yyexhaustedlab;
         YYSTACK_RELOCATE (yyss_alloc, yyss);
         YYSTACK_RELOCATE (yyvs_alloc, yyvs);
 #  undef YYSTACK_RELOCATE
         if (yyss1 != yyssa)
           YYSTACK_FREE (yyss1);
       }
 # endif
 #endif  
 
       yyssp = yyss + yysize - 1;
       yyvsp = yyvs + yysize - 1;
 
       YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                   (unsigned long int) yystacksize));
 
       if (yyss + yystacksize - 1 <= yyssp)
         YYABORT;
     }
 
   YYDPRINTF ((stderr, "Entering state %d\n", yystate));
 
   if (yystate == YYFINAL)
     YYACCEPT;
 
   goto yybackup;
 
  
 yybackup:
 
    
 
    
   yyn = yypact[yystate];
   if (yypact_value_is_default (yyn))
     goto yydefault;
 
    
 
    
   if (yychar == YYEMPTY)
     {
       YYDPRINTF ((stderr, "Reading a token: "));
       yychar = yylex (&yylval, yyscanner, lex_env);
     }
 
   if (yychar <= YYEOF)
     {
       yychar = yytoken = YYEOF;
       YYDPRINTF ((stderr, "Now at end of input.\n"));
     }
   else
     {
       yytoken = YYTRANSLATE (yychar);
       YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
     }
 
    
   yyn += yytoken;
   if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
     goto yydefault;
   yyn = yytable[yyn];
   if (yyn <= 0)
     {
       if (yytable_value_is_error (yyn))
         goto yyerrlab;
       yyn = -yyn;
       goto yyreduce;
     }
 
    
   if (yyerrstatus)
     yyerrstatus--;
 
    
   YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
 
    
   yychar = YYEMPTY;
 
   yystate = yyn;
   YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
   *++yyvsp = yylval;
   YY_IGNORE_MAYBE_UNINITIALIZED_END
 
   goto yynewstate;
 
 
  
 yydefault:
   yyn = yydefact[yystate];
   if (yyn == 0)
     goto yyerrlab;
   goto yyreduce;
 
 
  
 yyreduce:
    
   yylen = yyr2[yyn];
 
    
   yyval = yyvsp[1-yylen];
 
 
   YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
          case 2:
#line 105 "re_grammar.y"  
// #line 113 "re_grammar.y"  
      {
          RE_AST* re_ast = yyget_extra(yyscanner);
          re_ast->root_node = (yyvsp[0].re_node);
        }
#line 1340 "re_grammar.c"  
// #line 1348 "re_grammar.c"  
      break;
  
    case 4:
#line 114 "re_grammar.y"  
// #line 122 "re_grammar.y"  
      {
          (yyval.re_node) = (yyvsp[0].re_node);
        }
#line 1348 "re_grammar.c"  
// #line 1356 "re_grammar.c"  
      break;
  
    case 5:
#line 118 "re_grammar.y"  
// #line 126 "re_grammar.y"  
      {
          mark_as_not_fast_regexp();
//         incr_ast_levels();
  
          (yyval.re_node) = yr_re_node_create(RE_NODE_ALT, (yyvsp[-2].re_node), (yyvsp[0].re_node));
  
         DESTROY_NODE_IF((yyval.re_node) == NULL, (yyvsp[-2].re_node));
         DESTROY_NODE_IF((yyval.re_node) == NULL, (yyvsp[0].re_node));
  
          ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
        }
#line 1363 "re_grammar.c"  
// #line 1372 "re_grammar.c"  
      break;
  
    case 6:
#line 129 "re_grammar.y"  
// #line 138 "re_grammar.y"  
      {
          RE_NODE* node;
  
          mark_as_not_fast_regexp();
//         incr_ast_levels();
  
          node = yr_re_node_create(RE_NODE_EMPTY, NULL, NULL);
  
         DESTROY_NODE_IF((yyval.re_node) == NULL, (yyvsp[-1].re_node));
         ERROR_IF(node == NULL, ERROR_INSUFFICIENT_MEMORY);
 
         (yyval.re_node) = yr_re_node_create(RE_NODE_ALT, (yyvsp[-1].re_node), node);
  
          ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
        }
#line 1382 "re_grammar.c"  
// #line 1392 "re_grammar.c"  
      break;
  
    case 7:
#line 147 "re_grammar.y"  
// #line 157 "re_grammar.y"  
      {
          (yyval.re_node) = (yyvsp[0].re_node);
        }
#line 1390 "re_grammar.c"  
// #line 1400 "re_grammar.c"  
      break;
  
    case 8:
#line 151 "re_grammar.y"  
// #line 161 "re_grammar.y"  
      {
//         incr_ast_levels();
// 
          (yyval.re_node) = yr_re_node_create(RE_NODE_CONCAT, (yyvsp[-1].re_node), (yyvsp[0].re_node));
  
          DESTROY_NODE_IF((yyval.re_node) == NULL, (yyvsp[-1].re_node));
          DESTROY_NODE_IF((yyval.re_node) == NULL, (yyvsp[0].re_node));
          ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
        }
#line 1402 "re_grammar.c"  
// #line 1414 "re_grammar.c"  
      break;
  
    case 9:
#line 162 "re_grammar.y"  
// #line 174 "re_grammar.y"  
      {
          RE_AST* re_ast;
  
         mark_as_not_fast_regexp();
 
         re_ast = yyget_extra(yyscanner);
         re_ast->flags |= RE_FLAGS_GREEDY;
 
         (yyval.re_node) = yr_re_node_create(RE_NODE_STAR, (yyvsp[-1].re_node), NULL);
 
          DESTROY_NODE_IF((yyval.re_node) == NULL, (yyvsp[-1].re_node));
          ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
        }
#line 1420 "re_grammar.c"  
// #line 1432 "re_grammar.c"  
      break;
  
    case 10:
#line 176 "re_grammar.y"  
// #line 188 "re_grammar.y"  
      {
          RE_AST* re_ast;
  
         mark_as_not_fast_regexp();
 
         re_ast = yyget_extra(yyscanner);
         re_ast->flags |= RE_FLAGS_UNGREEDY;
 
         (yyval.re_node) = yr_re_node_create(RE_NODE_STAR, (yyvsp[-2].re_node), NULL);
 
         DESTROY_NODE_IF((yyval.re_node) == NULL, (yyvsp[-2].re_node));
         ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
  
          (yyval.re_node)->greedy = FALSE;
        }
#line 1440 "re_grammar.c"  
// #line 1452 "re_grammar.c"  
      break;
  
    case 11:
#line 192 "re_grammar.y"  
// #line 204 "re_grammar.y"  
      {
          RE_AST* re_ast;
  
         mark_as_not_fast_regexp();
 
         re_ast = yyget_extra(yyscanner);
         re_ast->flags |= RE_FLAGS_GREEDY;
 
         (yyval.re_node) = yr_re_node_create(RE_NODE_PLUS, (yyvsp[-1].re_node), NULL);
 
          DESTROY_NODE_IF((yyval.re_node) == NULL, (yyvsp[-1].re_node));
          ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
        }
#line 1458 "re_grammar.c"  
// #line 1470 "re_grammar.c"  
      break;
  
    case 12:
#line 206 "re_grammar.y"  
// #line 218 "re_grammar.y"  
      {
          RE_AST* re_ast;
  
         mark_as_not_fast_regexp();
 
         re_ast = yyget_extra(yyscanner);
         re_ast->flags |= RE_FLAGS_UNGREEDY;
 
         (yyval.re_node) = yr_re_node_create(RE_NODE_PLUS, (yyvsp[-2].re_node), NULL);
 
         DESTROY_NODE_IF((yyval.re_node) == NULL, (yyvsp[-2].re_node));
         ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
  
          (yyval.re_node)->greedy = FALSE;
        }
#line 1478 "re_grammar.c"  
// #line 1490 "re_grammar.c"  
      break;
  
    case 13:
#line 222 "re_grammar.y"  
// #line 234 "re_grammar.y"  
      {
          RE_AST* re_ast = yyget_extra(yyscanner);
          re_ast->flags |= RE_FLAGS_GREEDY;
 
         if ((yyvsp[-1].re_node)->type == RE_NODE_ANY)
         {
           (yyval.re_node) = yr_re_node_create(RE_NODE_RANGE_ANY, NULL, NULL);
           DESTROY_NODE_IF(TRUE, (yyvsp[-1].re_node));
         }
         else
         {
           mark_as_not_fast_regexp();
           (yyval.re_node) = yr_re_node_create(RE_NODE_RANGE, (yyvsp[-1].re_node), NULL);
           DESTROY_NODE_IF((yyval.re_node) == NULL, (yyvsp[-1].re_node));
         }
 
         DESTROY_NODE_IF((yyval.re_node) == NULL, (yyvsp[-1].re_node));
         ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
 
          (yyval.re_node)->start = 0;
          (yyval.re_node)->end = 1;
        }
#line 1505 "re_grammar.c"  
// #line 1517 "re_grammar.c"  
      break;
  
    case 14:
#line 245 "re_grammar.y"  
// #line 257 "re_grammar.y"  
      {
          RE_AST* re_ast = yyget_extra(yyscanner);
          re_ast->flags |= RE_FLAGS_UNGREEDY;
 
         if ((yyvsp[-2].re_node)->type == RE_NODE_ANY)
         {
           (yyval.re_node) = yr_re_node_create(RE_NODE_RANGE_ANY, NULL, NULL);
           DESTROY_NODE_IF(TRUE, (yyvsp[-2].re_node));
         }
         else
         {
           mark_as_not_fast_regexp();
           (yyval.re_node) = yr_re_node_create(RE_NODE_RANGE, (yyvsp[-2].re_node), NULL);
           DESTROY_NODE_IF((yyval.re_node) == NULL, (yyvsp[-2].re_node));
         }
 
         DESTROY_NODE_IF((yyval.re_node) == NULL, (yyvsp[-2].re_node));
         ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
 
         (yyval.re_node)->start = 0;
          (yyval.re_node)->end = 1;
          (yyval.re_node)->greedy = FALSE;
        }
#line 1533 "re_grammar.c"  
// #line 1545 "re_grammar.c"  
      break;
  
    case 15:
#line 269 "re_grammar.y"  
// #line 281 "re_grammar.y"  
      {
          RE_AST* re_ast = yyget_extra(yyscanner);
          re_ast->flags |= RE_FLAGS_GREEDY;
 
         if ((yyvsp[-1].re_node)->type == RE_NODE_ANY)
         {
           (yyval.re_node) = yr_re_node_create(RE_NODE_RANGE_ANY, NULL, NULL);
           DESTROY_NODE_IF(TRUE, (yyvsp[-1].re_node));
         }
         else
         {
           mark_as_not_fast_regexp();
           (yyval.re_node) = yr_re_node_create(RE_NODE_RANGE, (yyvsp[-1].re_node), NULL);
           DESTROY_NODE_IF((yyval.re_node) == NULL, (yyvsp[-1].re_node));
         }
 
         ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
 
          (yyval.re_node)->start = (yyvsp[0].range) & 0xFFFF;;
          (yyval.re_node)->end = (yyvsp[0].range) >> 16;;
        }
#line 1559 "re_grammar.c"  
// #line 1571 "re_grammar.c"  
      break;
  
    case 16:
#line 291 "re_grammar.y"  
// #line 303 "re_grammar.y"  
      {
          RE_AST* re_ast = yyget_extra(yyscanner);
          re_ast->flags |= RE_FLAGS_UNGREEDY;
 
         if ((yyvsp[-2].re_node)->type == RE_NODE_ANY)
         {
           (yyval.re_node) = yr_re_node_create(RE_NODE_RANGE_ANY, NULL, NULL);
           DESTROY_NODE_IF(TRUE, (yyvsp[-2].re_node));
         }
         else
         {
           mark_as_not_fast_regexp();
           (yyval.re_node) = yr_re_node_create(RE_NODE_RANGE, (yyvsp[-2].re_node), NULL);
           DESTROY_NODE_IF((yyval.re_node) == NULL, (yyvsp[-2].re_node));
         }
 
         ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
 
         (yyval.re_node)->start = (yyvsp[-1].range) & 0xFFFF;;
          (yyval.re_node)->end = (yyvsp[-1].range) >> 16;;
          (yyval.re_node)->greedy = FALSE;
        }
#line 1586 "re_grammar.c"  
// #line 1598 "re_grammar.c"  
      break;
  
    case 17:
#line 314 "re_grammar.y"  
// #line 326 "re_grammar.y"  
      {
          (yyval.re_node) = (yyvsp[0].re_node);
        }
#line 1594 "re_grammar.c"  
// #line 1606 "re_grammar.c"  
      break;
  
    case 18:
#line 318 "re_grammar.y"  
// #line 330 "re_grammar.y"  
      {
          (yyval.re_node) = yr_re_node_create(RE_NODE_WORD_BOUNDARY, NULL, NULL);
  
          ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
        }
#line 1604 "re_grammar.c"  
// #line 1616 "re_grammar.c"  
      break;
  
    case 19:
#line 324 "re_grammar.y"  
// #line 336 "re_grammar.y"  
      {
          (yyval.re_node) = yr_re_node_create(RE_NODE_NON_WORD_BOUNDARY, NULL, NULL);
  
          ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
        }
#line 1614 "re_grammar.c"  
// #line 1626 "re_grammar.c"  
      break;
  
    case 20:
#line 330 "re_grammar.y"  
// #line 342 "re_grammar.y"  
      {
          (yyval.re_node) = yr_re_node_create(RE_NODE_ANCHOR_START, NULL, NULL);
  
          ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
        }
#line 1624 "re_grammar.c"  
// #line 1636 "re_grammar.c"  
      break;
  
    case 21:
#line 336 "re_grammar.y"  
// #line 348 "re_grammar.y"  
      {
          (yyval.re_node) = yr_re_node_create(RE_NODE_ANCHOR_END, NULL, NULL);
  
          ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
        }
#line 1634 "re_grammar.c"  
// #line 1646 "re_grammar.c"  
      break;
  
    case 22:
#line 345 "re_grammar.y"  
// #line 357 "re_grammar.y"  
      {
//         incr_ast_levels();
// 
          (yyval.re_node) = (yyvsp[-1].re_node);
        }
#line 1642 "re_grammar.c"  
// #line 1656 "re_grammar.c"  
      break;
  
    case 23:
#line 349 "re_grammar.y"  
// #line 363 "re_grammar.y"  
      {
          (yyval.re_node) = yr_re_node_create(RE_NODE_ANY, NULL, NULL);
  
          ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
        }
#line 1652 "re_grammar.c"  
// #line 1666 "re_grammar.c"  
      break;
  
    case 24:
#line 355 "re_grammar.y"  
// #line 369 "re_grammar.y"  
      {
          (yyval.re_node) = yr_re_node_create(RE_NODE_LITERAL, NULL, NULL);
  
          ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
  
          (yyval.re_node)->value = (yyvsp[0].integer);
        }
#line 1664 "re_grammar.c"  
// #line 1678 "re_grammar.c"  
      break;
  
    case 25:
#line 363 "re_grammar.y"  
// #line 377 "re_grammar.y"  
      {
          (yyval.re_node) = yr_re_node_create(RE_NODE_WORD_CHAR, NULL, NULL);
  
          ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
        }
#line 1674 "re_grammar.c"  
// #line 1688 "re_grammar.c"  
      break;
  
    case 26:
#line 369 "re_grammar.y"  
// #line 383 "re_grammar.y"  
      {
          (yyval.re_node) = yr_re_node_create(RE_NODE_NON_WORD_CHAR, NULL, NULL);
  
          ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
        }
#line 1684 "re_grammar.c"  
// #line 1698 "re_grammar.c"  
      break;
  
    case 27:
#line 375 "re_grammar.y"  
// #line 389 "re_grammar.y"  
      {
          (yyval.re_node) = yr_re_node_create(RE_NODE_SPACE, NULL, NULL);
  
          ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
        }
#line 1694 "re_grammar.c"  
// #line 1708 "re_grammar.c"  
      break;
  
    case 28:
#line 381 "re_grammar.y"  
// #line 395 "re_grammar.y"  
      {
           (yyval.re_node) = yr_re_node_create(RE_NODE_NON_SPACE, NULL, NULL);
  
           ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
        }
#line 1704 "re_grammar.c"  
// #line 1718 "re_grammar.c"  
      break;
  
    case 29:
#line 387 "re_grammar.y"  
// #line 401 "re_grammar.y"  
      {
          (yyval.re_node) = yr_re_node_create(RE_NODE_DIGIT, NULL, NULL);
  
          ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
        }
#line 1714 "re_grammar.c"  
// #line 1728 "re_grammar.c"  
      break;
  
    case 30:
#line 393 "re_grammar.y"  
// #line 407 "re_grammar.y"  
      {
          (yyval.re_node) = yr_re_node_create(RE_NODE_NON_DIGIT, NULL, NULL);
  
          ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
        }
#line 1724 "re_grammar.c"  
// #line 1738 "re_grammar.c"  
      break;
  
    case 31:
#line 399 "re_grammar.y"  
// #line 413 "re_grammar.y"  
      {
          (yyval.re_node) = yr_re_node_create(RE_NODE_CLASS, NULL, NULL);
  
          ERROR_IF((yyval.re_node) == NULL, ERROR_INSUFFICIENT_MEMORY);
  
          (yyval.re_node)->class_vector = (yyvsp[0].class_vector);
        }
#line 1736 "re_grammar.c"  
// #line 1750 "re_grammar.c"  
      break;
  
  
#line 1740 "re_grammar.c"  
// #line 1754 "re_grammar.c"  
        default: break;
      }
     
   YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);
 
   YYPOPSTACK (yylen);
   yylen = 0;
   YY_STACK_PRINT (yyss, yyssp);
 
   *++yyvsp = yyval;
 
    
 
   yyn = yyr1[yyn];
 
   yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
   if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
     yystate = yytable[yystate];
   else
     yystate = yydefgoto[yyn - YYNTOKENS];
 
   goto yynewstate;
 
 
  
 yyerrlab:
    
   yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);
 
    
   if (!yyerrstatus)
     {
       ++yynerrs;
 #if ! YYERROR_VERBOSE
       yyerror (yyscanner, lex_env, YY_("syntax error"));
 #else
 # define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                         yyssp, yytoken)
       {
         char const *yymsgp = YY_("syntax error");
         int yysyntax_error_status;
         yysyntax_error_status = YYSYNTAX_ERROR;
         if (yysyntax_error_status == 0)
           yymsgp = yymsg;
         else if (yysyntax_error_status == 1)
           {
             if (yymsg != yymsgbuf)
               YYSTACK_FREE (yymsg);
             yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
             if (!yymsg)
               {
                 yymsg = yymsgbuf;
                 yymsg_alloc = sizeof yymsgbuf;
                 yysyntax_error_status = 2;
               }
             else
               {
                 yysyntax_error_status = YYSYNTAX_ERROR;
                 yymsgp = yymsg;
               }
           }
         yyerror (yyscanner, lex_env, yymsgp);
         if (yysyntax_error_status == 2)
           goto yyexhaustedlab;
       }
 # undef YYSYNTAX_ERROR
 #endif
     }
 
 
 
   if (yyerrstatus == 3)
     {
        
 
       if (yychar <= YYEOF)
         {
            
           if (yychar == YYEOF)
             YYABORT;
         }
       else
         {
           yydestruct ("Error: discarding",
                       yytoken, &yylval, yyscanner, lex_env);
           yychar = YYEMPTY;
         }
     }
 
    
   goto yyerrlab1;
 
 
  
 yyerrorlab:
 
    
   if (  0)
      goto yyerrorlab;
 
    
   YYPOPSTACK (yylen);
   yylen = 0;
   YY_STACK_PRINT (yyss, yyssp);
   yystate = *yyssp;
   goto yyerrlab1;
 
 
  
 yyerrlab1:
   yyerrstatus = 3;       
 
   for (;;)
     {
       yyn = yypact[yystate];
       if (!yypact_value_is_default (yyn))
         {
           yyn += YYTERROR;
           if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
             {
               yyn = yytable[yyn];
               if (0 < yyn)
                 break;
             }
         }
 
        
       if (yyssp == yyss)
         YYABORT;
 
 
       yydestruct ("Error: popping",
                   yystos[yystate], yyvsp, yyscanner, lex_env);
       YYPOPSTACK (1);
       yystate = *yyssp;
       YY_STACK_PRINT (yyss, yyssp);
     }
 
   YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
   *++yyvsp = yylval;
   YY_IGNORE_MAYBE_UNINITIALIZED_END
 
 
    
   YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);
 
   yystate = yyn;
   goto yynewstate;
 
 
  
 yyacceptlab:
   yyresult = 0;
   goto yyreturn;
 
  
 yyabortlab:
   yyresult = 1;
   goto yyreturn;
 
 #if !defined yyoverflow || YYERROR_VERBOSE
  
 yyexhaustedlab:
   yyerror (yyscanner, lex_env, YY_("memory exhausted"));
   yyresult = 2;
    
 #endif
 
 yyreturn:
   if (yychar != YYEMPTY)
     {
        
       yytoken = YYTRANSLATE (yychar);
       yydestruct ("Cleanup: discarding lookahead",
                   yytoken, &yylval, yyscanner, lex_env);
     }
    
   YYPOPSTACK (yylen);
   YY_STACK_PRINT (yyss, yyssp);
   while (yyssp != yyss)
     {
       yydestruct ("Cleanup: popping",
                   yystos[*yyssp], yyvsp, yyscanner, lex_env);
       YYPOPSTACK (1);
     }
 #ifndef yyoverflow
   if (yyss != yyssa)
     YYSTACK_FREE (yyss);
 #endif
 #if YYERROR_VERBOSE
   if (yymsg != yymsgbuf)
     YYSTACK_FREE (yymsg);
  #endif
    return yyresult;
  }