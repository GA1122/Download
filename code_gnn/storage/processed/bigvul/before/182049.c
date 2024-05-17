 yyparse (void *YYPARSE_PARAM)
 #else
 int
 yyparse (YYPARSE_PARAM)
     void *YYPARSE_PARAM;
 #endif
 #else  
 #if (defined __STDC__ || defined __C99__FUNC__ \
      || defined __cplusplus || defined _MSC_VER)
 int
 yyparse (void)
 #else
 int
 yyparse ()
 
 #endif
 #endif
 {
 
 
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
    
   int yytoken;
    
   YYSTYPE yyval;
 
 #if YYERROR_VERBOSE
    
   char yymsgbuf[128];
   char *yymsg = yymsgbuf;
   YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
 #endif
 
 #define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))
 
    
   int yylen = 0;
 
   yytoken = 0;
   yyss = yyssa;
   yyvs = yyvsa;
   yystacksize = YYINITDEPTH;
 
   YYDPRINTF ((stderr, "Starting parse\n"));
 
   yystate = 0;
   yyerrstatus = 0;
   yynerrs = 0;
   yychar = YYEMPTY;  
 
    
   yyssp = yyss;
   yyvsp = yyvs;
 
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
   if (yyn == YYPACT_NINF)
     goto yydefault;
 
    
 
    
   if (yychar == YYEMPTY)
     {
       YYDPRINTF ((stderr, "Reading a token: "));
       yychar = YYLEX;
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
       if (yyn == 0 || yyn == YYTABLE_NINF)
 	goto yyerrlab;
       yyn = -yyn;
       goto yyreduce;
     }
 
    
   if (yyerrstatus)
     yyerrstatus--;
 
    
   YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
 
    
   yychar = YYEMPTY;
 
   yystate = yyn;
   *++yyvsp = yylval;
 
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
         case 5:
 
  
 #line 320 "ntp_parser.y"
     {
 			 
 			msyslog(LOG_ERR, 
 				"syntax error in %s line %d, column %d",
 				ip_file->fname,
 				ip_file->err_line_no,
 				ip_file->err_col_no);
 		}
     break;
 
   case 19:
 
  
 #line 354 "ntp_parser.y"
     {
 			struct peer_node *my_node =  create_peer_node((yyvsp[(1) - (3)].Integer), (yyvsp[(2) - (3)].Address_node), (yyvsp[(3) - (3)].Queue));
 			if (my_node)
 				enqueue(cfgt.peers, my_node);
 		}
     break;
 
   case 20:
 
  
 #line 360 "ntp_parser.y"
     {
 			struct peer_node *my_node = create_peer_node((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Address_node), NULL);
 			if (my_node)
 				enqueue(cfgt.peers, my_node);
 		}
     break;
 
   case 27:
 
  
 #line 377 "ntp_parser.y"
     { (yyval.Address_node) = create_address_node((yyvsp[(2) - (2)].String), AF_INET); }
     break;
 
   case 28:
 
  
 #line 378 "ntp_parser.y"
     { (yyval.Address_node) = create_address_node((yyvsp[(2) - (2)].String), AF_INET6); }
     break;
 
   case 29:
 
  
 #line 382 "ntp_parser.y"
     { (yyval.Address_node) = create_address_node((yyvsp[(1) - (1)].String), 0); }
     break;
 
   case 30:
 
  
 #line 386 "ntp_parser.y"
     { (yyval.Queue) = enqueue((yyvsp[(1) - (2)].Queue), (yyvsp[(2) - (2)].Attr_val)); }
     break;
 
   case 31:
 
  
 #line 387 "ntp_parser.y"
     { (yyval.Queue) = enqueue_in_new_queue((yyvsp[(1) - (1)].Attr_val)); }
     break;
 
   case 32:
 
  
 #line 391 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival(T_Flag, (yyvsp[(1) - (1)].Integer)); }
     break;
 
   case 33:
 
  
 #line 392 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Double)); }
     break;
 
   case 34:
 
  
 #line 393 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival(T_Flag, (yyvsp[(1) - (1)].Integer)); }
     break;
 
   case 35:
 
  
 #line 394 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival(T_Flag, (yyvsp[(1) - (1)].Integer)); }
     break;
 
   case 36:
 
  
 #line 395 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 37:
 
  
 #line 396 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 38:
 
  
 #line 397 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 39:
 
  
 #line 398 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival(T_Flag, (yyvsp[(1) - (1)].Integer)); }
     break;
 
   case 40:
 
  
 #line 399 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival(T_Flag, (yyvsp[(1) - (1)].Integer)); }
     break;
 
   case 41:
 
  
 #line 400 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival(T_Flag, (yyvsp[(1) - (1)].Integer)); }
     break;
 
   case 42:
 
  
 #line 401 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival(T_Flag, (yyvsp[(1) - (1)].Integer)); }
     break;
 
   case 43:
 
  
 #line 402 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival(T_Flag, (yyvsp[(1) - (1)].Integer)); }
     break;
 
   case 44:
 
  
 #line 403 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 45:
 
  
 #line 404 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 46:
 
  
 #line 405 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 47:
 
  
 #line 415 "ntp_parser.y"
     {
 			struct unpeer_node *my_node = create_unpeer_node((yyvsp[(2) - (2)].Address_node));
 			if (my_node)
 				enqueue(cfgt.unpeers, my_node);
 		}
     break;
 
   case 50:
 
  
 #line 434 "ntp_parser.y"
     { cfgt.broadcastclient = 1; }
     break;
 
   case 51:
 
  
 #line 436 "ntp_parser.y"
     { append_queue(cfgt.manycastserver, (yyvsp[(2) - (2)].Queue)); }
     break;
 
   case 52:
 
  
 #line 438 "ntp_parser.y"
     { append_queue(cfgt.multicastclient, (yyvsp[(2) - (2)].Queue)); }
     break;
 
   case 53:
 
  
 #line 449 "ntp_parser.y"
     { enqueue(cfgt.vars, create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer))); }
     break;
 
   case 54:
 
  
 #line 451 "ntp_parser.y"
     { cfgt.auth.control_key = (yyvsp[(2) - (2)].Integer); }
     break;
 
   case 55:
 
  
 #line 453 "ntp_parser.y"
     { 
 			cfgt.auth.cryptosw++;
 			append_queue(cfgt.auth.crypto_cmd_list, (yyvsp[(2) - (2)].Queue));
 		}
     break;
 
   case 56:
 
  
 #line 458 "ntp_parser.y"
     { cfgt.auth.keys = (yyvsp[(2) - (2)].String); }
     break;
 
   case 57:
 
  
 #line 460 "ntp_parser.y"
     { cfgt.auth.keysdir = (yyvsp[(2) - (2)].String); }
     break;
 
   case 58:
 
  
 #line 462 "ntp_parser.y"
     { cfgt.auth.request_key = (yyvsp[(2) - (2)].Integer); }
     break;
 
   case 59:
 
  
 #line 464 "ntp_parser.y"
     { cfgt.auth.revoke = (yyvsp[(2) - (2)].Integer); }
     break;
 
   case 60:
 
  
 #line 466 "ntp_parser.y"
     { cfgt.auth.trusted_key_list = (yyvsp[(2) - (2)].Queue); }
     break;
 
   case 61:
 
  
 #line 468 "ntp_parser.y"
     { cfgt.auth.ntp_signd_socket = (yyvsp[(2) - (2)].String); }
     break;
 
   case 63:
 
  
 #line 474 "ntp_parser.y"
     { (yyval.Queue) = create_queue(); }
     break;
 
   case 64:
 
  
 #line 479 "ntp_parser.y"
     { 
 			if ((yyvsp[(2) - (2)].Attr_val) != NULL)
 				(yyval.Queue) = enqueue((yyvsp[(1) - (2)].Queue), (yyvsp[(2) - (2)].Attr_val));
 			else
 				(yyval.Queue) = (yyvsp[(1) - (2)].Queue);
 		}
     break;
 
   case 65:
 
  
 #line 486 "ntp_parser.y"
     {
 			if ((yyvsp[(1) - (1)].Attr_val) != NULL)
 				(yyval.Queue) = enqueue_in_new_queue((yyvsp[(1) - (1)].Attr_val));
 			else
 				(yyval.Queue) = create_queue();
 		}
     break;
 
   case 66:
 
  
 #line 496 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_sval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].String)); }
     break;
 
   case 67:
 
  
 #line 498 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_sval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].String)); }
     break;
 
   case 68:
 
  
 #line 500 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_sval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].String)); }
     break;
 
   case 69:
 
  
 #line 502 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_sval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].String)); }
     break;
 
   case 70:
 
  
 #line 504 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_sval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].String)); }
     break;
 
   case 71:
 
  
 #line 506 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_sval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].String)); }
     break;
 
   case 72:
 
  
 #line 508 "ntp_parser.y"
     {
 			(yyval.Attr_val) = NULL;
 			cfgt.auth.revoke = (yyvsp[(2) - (2)].Integer);
 			msyslog(LOG_WARNING,
 				"'crypto revoke %d' is deprecated, "
 				"please use 'revoke %d' instead.",
 				cfgt.auth.revoke, cfgt.auth.revoke);
 		}
     break;
 
   case 73:
 
  
 #line 525 "ntp_parser.y"
     { append_queue(cfgt.orphan_cmds,(yyvsp[(2) - (2)].Queue)); }
     break;
 
   case 74:
 
  
 #line 529 "ntp_parser.y"
     { (yyval.Queue) = enqueue((yyvsp[(1) - (2)].Queue), (yyvsp[(2) - (2)].Attr_val)); }
     break;
 
   case 75:
 
  
 #line 530 "ntp_parser.y"
     { (yyval.Queue) = enqueue_in_new_queue((yyvsp[(1) - (1)].Attr_val)); }
     break;
 
   case 76:
 
  
 #line 535 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (double)(yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 77:
 
  
 #line 537 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (double)(yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 78:
 
  
 #line 539 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (double)(yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 79:
 
  
 #line 541 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (double)(yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 80:
 
  
 #line 543 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (double)(yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 81:
 
  
 #line 545 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Double)); }
     break;
 
   case 82:
 
  
 #line 547 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Double)); }
     break;
 
   case 83:
 
  
 #line 549 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Double)); }
     break;
 
   case 84:
 
  
 #line 551 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Double)); }
     break;
 
   case 85:
 
  
 #line 553 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (double)(yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 86:
 
  
 #line 555 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (double)(yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 87:
 
  
 #line 565 "ntp_parser.y"
     { append_queue(cfgt.stats_list, (yyvsp[(2) - (2)].Queue)); }
     break;
 
   case 88:
 
  
 #line 567 "ntp_parser.y"
     {
 			if (input_from_file)
 				cfgt.stats_dir = (yyvsp[(2) - (2)].String);
 			else {
 				free((yyvsp[(2) - (2)].String));
 				yyerror("statsdir remote configuration ignored");
 			}
 		}
     break;
 
   case 89:
 
  
 #line 576 "ntp_parser.y"
     {
 			enqueue(cfgt.filegen_opts,
 				create_filegen_node((yyvsp[(2) - (3)].Integer), (yyvsp[(3) - (3)].Queue)));
 		}
     break;
 
   case 90:
 
  
 #line 583 "ntp_parser.y"
     { (yyval.Queue) = enqueue((yyvsp[(1) - (2)].Queue), create_ival((yyvsp[(2) - (2)].Integer))); }
     break;
 
   case 91:
 
  
 #line 584 "ntp_parser.y"
     { (yyval.Queue) = enqueue_in_new_queue(create_ival((yyvsp[(1) - (1)].Integer))); }
     break;
 
   case 100:
 
  
 #line 600 "ntp_parser.y"
     {
 			if ((yyvsp[(2) - (2)].Attr_val) != NULL)
 				(yyval.Queue) = enqueue((yyvsp[(1) - (2)].Queue), (yyvsp[(2) - (2)].Attr_val));
 			else
 				(yyval.Queue) = (yyvsp[(1) - (2)].Queue);
 		}
     break;
 
   case 101:
 
  
 #line 607 "ntp_parser.y"
     {
 			if ((yyvsp[(1) - (1)].Attr_val) != NULL)
 				(yyval.Queue) = enqueue_in_new_queue((yyvsp[(1) - (1)].Attr_val));
 			else
 				(yyval.Queue) = create_queue();
 		}
     break;
 
   case 102:
 
  
 #line 617 "ntp_parser.y"
     {
 			if (input_from_file)
 				(yyval.Attr_val) = create_attr_sval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].String));
 			else {
 				(yyval.Attr_val) = NULL;
 				free((yyvsp[(2) - (2)].String));
 				yyerror("filegen file remote configuration ignored");
 			}
 		}
     break;
 
   case 103:
 
  
 #line 627 "ntp_parser.y"
     {
 			if (input_from_file)
 				(yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer));
 			else {
 				(yyval.Attr_val) = NULL;
 				yyerror("filegen type remote configuration ignored");
 			}
 		}
     break;
 
   case 104:
 
  
 #line 636 "ntp_parser.y"
     {
 			if (input_from_file)
 				(yyval.Attr_val) = create_attr_ival(T_Flag, (yyvsp[(1) - (1)].Integer));
 			else {
 				(yyval.Attr_val) = NULL;
 				yyerror("filegen link remote configuration ignored");
 			}
 		}
     break;
 
   case 105:
 
  
 #line 645 "ntp_parser.y"
     {
 			if (input_from_file)
 				(yyval.Attr_val) = create_attr_ival(T_Flag, (yyvsp[(1) - (1)].Integer));
 			else {
 				(yyval.Attr_val) = NULL;
 				yyerror("filegen nolink remote configuration ignored");
 			}
 		}
     break;
 
   case 106:
 
  
 #line 653 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival(T_Flag, (yyvsp[(1) - (1)].Integer)); }
     break;
 
   case 107:
 
  
 #line 654 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival(T_Flag, (yyvsp[(1) - (1)].Integer)); }
     break;
 
   case 115:
 
  
 #line 674 "ntp_parser.y"
     {
 			append_queue(cfgt.discard_opts, (yyvsp[(2) - (2)].Queue));
 		}
     break;
 
   case 116:
 
  
 #line 678 "ntp_parser.y"
     {
 			append_queue(cfgt.mru_opts, (yyvsp[(2) - (2)].Queue));
 		}
     break;
 
   case 117:
 
  
 #line 682 "ntp_parser.y"
     {
 			enqueue(cfgt.restrict_opts,
 				create_restrict_node((yyvsp[(2) - (3)].Address_node), NULL, (yyvsp[(3) - (3)].Queue), ip_file->line_no));
 		}
     break;
 
   case 118:
 
  
 #line 687 "ntp_parser.y"
     {
 			enqueue(cfgt.restrict_opts,
 				create_restrict_node((yyvsp[(2) - (5)].Address_node), (yyvsp[(4) - (5)].Address_node), (yyvsp[(5) - (5)].Queue), ip_file->line_no));
 		}
     break;
 
   case 119:
 
  
 #line 692 "ntp_parser.y"
     {
 			enqueue(cfgt.restrict_opts,
 				create_restrict_node(NULL, NULL, (yyvsp[(3) - (3)].Queue), ip_file->line_no));
 		}
     break;
 
   case 120:
 
  
 #line 697 "ntp_parser.y"
     {
 			enqueue(cfgt.restrict_opts,
 				create_restrict_node(
 					create_address_node(
 						estrdup("0.0.0.0"), 
 						AF_INET),
 					create_address_node(
 						estrdup("0.0.0.0"), 
 						AF_INET),
 					(yyvsp[(4) - (4)].Queue), 
 					ip_file->line_no));
 		}
     break;
 
   case 121:
 
  
 #line 710 "ntp_parser.y"
     {
 			enqueue(cfgt.restrict_opts,
 				create_restrict_node(
 					create_address_node(
 						estrdup("::"), 
 						AF_INET6),
 					create_address_node(
 						estrdup("::"), 
 						AF_INET6),
 					(yyvsp[(4) - (4)].Queue), 
 					ip_file->line_no));
 		}
     break;
 
   case 122:
 
  
 #line 723 "ntp_parser.y"
     {
 			enqueue(cfgt.restrict_opts,
 				create_restrict_node(
 					NULL, NULL,
 					enqueue((yyvsp[(3) - (3)].Queue), create_ival((yyvsp[(2) - (3)].Integer))),
 					ip_file->line_no));
 		}
     break;
 
   case 123:
 
  
 #line 734 "ntp_parser.y"
     { (yyval.Queue) = create_queue(); }
     break;
 
   case 124:
 
  
 #line 736 "ntp_parser.y"
     { (yyval.Queue) = enqueue((yyvsp[(1) - (2)].Queue), create_ival((yyvsp[(2) - (2)].Integer))); }
     break;
 
   case 139:
 
  
 #line 758 "ntp_parser.y"
     { (yyval.Queue) = enqueue((yyvsp[(1) - (2)].Queue), (yyvsp[(2) - (2)].Attr_val)); }
     break;
 
   case 140:
 
  
 #line 760 "ntp_parser.y"
     { (yyval.Queue) = enqueue_in_new_queue((yyvsp[(1) - (1)].Attr_val)); }
     break;
 
   case 141:
 
  
 #line 764 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 142:
 
  
 #line 765 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 143:
 
  
 #line 766 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 144:
 
  
 #line 771 "ntp_parser.y"
     { (yyval.Queue) = enqueue((yyvsp[(1) - (2)].Queue), (yyvsp[(2) - (2)].Attr_val)); }
     break;
 
   case 145:
 
  
 #line 773 "ntp_parser.y"
     { (yyval.Queue) = enqueue_in_new_queue((yyvsp[(1) - (1)].Attr_val)); }
     break;
 
   case 146:
 
  
 #line 777 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 147:
 
  
 #line 778 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 148:
 
  
 #line 779 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 149:
 
  
 #line 780 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 150:
 
  
 #line 781 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 151:
 
  
 #line 782 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 152:
 
  
 #line 783 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 153:
 
  
 #line 784 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 154:
 
  
 #line 793 "ntp_parser.y"
     { enqueue(cfgt.fudge, create_addr_opts_node((yyvsp[(2) - (3)].Address_node), (yyvsp[(3) - (3)].Queue))); }
     break;
 
   case 155:
 
  
 #line 798 "ntp_parser.y"
     { enqueue((yyvsp[(1) - (2)].Queue), (yyvsp[(2) - (2)].Attr_val)); }
     break;
 
   case 156:
 
  
 #line 800 "ntp_parser.y"
     { (yyval.Queue) = enqueue_in_new_queue((yyvsp[(1) - (1)].Attr_val)); }
     break;
 
   case 157:
 
  
 #line 804 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Double)); }
     break;
 
   case 158:
 
  
 #line 805 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Double)); }
     break;
 
   case 159:
 
  
 #line 806 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 160:
 
  
 #line 807 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_sval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].String)); }
     break;
 
   case 161:
 
  
 #line 808 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 162:
 
  
 #line 809 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 163:
 
  
 #line 810 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 164:
 
  
 #line 811 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 165:
 
  
 #line 820 "ntp_parser.y"
     { append_queue(cfgt.enable_opts, (yyvsp[(2) - (2)].Queue));  }
     break;
 
   case 166:
 
  
 #line 822 "ntp_parser.y"
     { append_queue(cfgt.disable_opts, (yyvsp[(2) - (2)].Queue));  }
     break;
 
   case 167:
 
  
 #line 827 "ntp_parser.y"
     {
 			if ((yyvsp[(2) - (2)].Attr_val) != NULL)
 				(yyval.Queue) = enqueue((yyvsp[(1) - (2)].Queue), (yyvsp[(2) - (2)].Attr_val));
 			else
 				(yyval.Queue) = (yyvsp[(1) - (2)].Queue);
 		}
     break;
 
   case 168:
 
  
 #line 834 "ntp_parser.y"
     {
 			if ((yyvsp[(1) - (1)].Attr_val) != NULL)
 				(yyval.Queue) = enqueue_in_new_queue((yyvsp[(1) - (1)].Attr_val));
 			else
 				(yyval.Queue) = create_queue();
 		}
     break;
 
   case 169:
 
  
 #line 843 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival(T_Flag, (yyvsp[(1) - (1)].Integer)); }
     break;
 
   case 170:
 
  
 #line 844 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival(T_Flag, (yyvsp[(1) - (1)].Integer)); }
     break;
 
   case 171:
 
  
 #line 845 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival(T_Flag, (yyvsp[(1) - (1)].Integer)); }
     break;
 
   case 172:
 
  
 #line 846 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival(T_Flag, (yyvsp[(1) - (1)].Integer)); }
     break;
 
   case 173:
 
  
 #line 847 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival(T_Flag, (yyvsp[(1) - (1)].Integer)); }
     break;
 
   case 174:
 
  
 #line 848 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival(T_Flag, (yyvsp[(1) - (1)].Integer)); }
     break;
 
   case 175:
 
  
 #line 850 "ntp_parser.y"
     { 
 			if (input_from_file)
 				(yyval.Attr_val) = create_attr_ival(T_Flag, (yyvsp[(1) - (1)].Integer));
 			else {
 				(yyval.Attr_val) = NULL;
 				yyerror("enable/disable stats remote configuration ignored");
 			}
 		}
     break;
 
   case 176:
 
  
 #line 865 "ntp_parser.y"
     { append_queue(cfgt.tinker, (yyvsp[(2) - (2)].Queue)); }
     break;
 
   case 177:
 
  
 #line 869 "ntp_parser.y"
     { (yyval.Queue) = enqueue((yyvsp[(1) - (2)].Queue), (yyvsp[(2) - (2)].Attr_val)); }
     break;
 
   case 178:
 
  
 #line 870 "ntp_parser.y"
     { (yyval.Queue) = enqueue_in_new_queue((yyvsp[(1) - (1)].Attr_val)); }
     break;
 
   case 179:
 
  
 #line 874 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Double)); }
     break;
 
   case 180:
 
  
 #line 875 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Double)); }
     break;
 
   case 181:
 
  
 #line 876 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Double)); }
     break;
 
   case 182:
 
  
 #line 877 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Double)); }
     break;
 
   case 183:
 
  
 #line 878 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Double)); }
     break;
 
   case 184:
 
  
 #line 879 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Double)); }
     break;
 
   case 185:
 
  
 #line 880 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Double)); }
     break;
 
   case 187:
 
  
 #line 891 "ntp_parser.y"
     {
 			if (curr_include_level >= MAXINCLUDELEVEL) {
 				fprintf(stderr, "getconfig: Maximum include file level exceeded.\n");
 				msyslog(LOG_ERR, "getconfig: Maximum include file level exceeded.");
 			}
 			else {
 				fp[curr_include_level + 1] = F_OPEN(FindConfig((yyvsp[(2) - (3)].String)), "r");
 				if (fp[curr_include_level + 1] == NULL) {
 					fprintf(stderr, "getconfig: Couldn't open <%s>\n", FindConfig((yyvsp[(2) - (3)].String)));
 					msyslog(LOG_ERR, "getconfig: Couldn't open <%s>", FindConfig((yyvsp[(2) - (3)].String)));
 				}
 				else
 					ip_file = fp[++curr_include_level];
 			}
 		}
     break;
 
   case 188:
 
  
 #line 907 "ntp_parser.y"
     {
 			while (curr_include_level != -1)
 				FCLOSE(fp[curr_include_level--]);
 		}
     break;
 
   case 189:
 
  
 #line 913 "ntp_parser.y"
     { enqueue(cfgt.vars, create_attr_dval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Double))); }
     break;
 
   case 190:
 
  
 #line 915 "ntp_parser.y"
     { enqueue(cfgt.vars, create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer))); }
     break;
 
   case 191:
 
  
 #line 917 "ntp_parser.y"
     { enqueue(cfgt.vars, create_attr_dval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Double))); }
     break;
 
   case 192:
 
  
 #line 919 "ntp_parser.y"
     {   }
     break;
 
   case 193:
 
  
 #line 921 "ntp_parser.y"
     { enqueue(cfgt.vars, create_attr_sval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].String))); }
     break;
 
   case 194:
 
  
 #line 924 "ntp_parser.y"
     { enqueue(cfgt.vars, create_attr_sval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].String))); }
     break;
 
   case 195:
 
  
 #line 926 "ntp_parser.y"
     {
 			if (input_from_file)
 				enqueue(cfgt.vars,
 					create_attr_sval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].String)));
 			else {
 				free((yyvsp[(2) - (2)].String));
 				yyerror("logfile remote configuration ignored");
 			}
 		}
     break;
 
   case 196:
 
  
 #line 937 "ntp_parser.y"
     { append_queue(cfgt.logconfig, (yyvsp[(2) - (2)].Queue)); }
     break;
 
   case 197:
 
  
 #line 939 "ntp_parser.y"
     { append_queue(cfgt.phone, (yyvsp[(2) - (2)].Queue)); }
     break;
 
   case 198:
 
  
 #line 941 "ntp_parser.y"
     {
 			if (input_from_file)
 				enqueue(cfgt.vars,
 					create_attr_sval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].String)));
 			else {
 				free((yyvsp[(2) - (2)].String));
 				yyerror("saveconfigdir remote configuration ignored");
 			}
 		}
     break;
 
   case 199:
 
  
 #line 951 "ntp_parser.y"
     { enqueue(cfgt.setvar, (yyvsp[(2) - (2)].Set_var)); }
     break;
 
   case 200:
 
  
 #line 953 "ntp_parser.y"
     { enqueue(cfgt.trap, create_addr_opts_node((yyvsp[(2) - (2)].Address_node), NULL)); }
     break;
 
   case 201:
 
  
 #line 955 "ntp_parser.y"
     { enqueue(cfgt.trap, create_addr_opts_node((yyvsp[(2) - (3)].Address_node), (yyvsp[(3) - (3)].Queue))); }
     break;
 
   case 202:
 
  
 #line 957 "ntp_parser.y"
     { append_queue(cfgt.ttl, (yyvsp[(2) - (2)].Queue)); }
     break;
 
   case 203:
 
  
 #line 959 "ntp_parser.y"
     { enqueue(cfgt.qos, create_attr_sval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].String))); }
     break;
 
   case 204:
 
  
 #line 964 "ntp_parser.y"
     { enqueue(cfgt.vars, create_attr_sval(T_Driftfile, (yyvsp[(1) - (1)].String))); }
     break;
 
   case 205:
 
  
 #line 966 "ntp_parser.y"
     { enqueue(cfgt.vars, create_attr_dval(T_WanderThreshold, (yyvsp[(2) - (2)].Double)));
 			  enqueue(cfgt.vars, create_attr_sval(T_Driftfile, (yyvsp[(1) - (2)].String))); }
     break;
 
   case 206:
 
  
 #line 969 "ntp_parser.y"
     { enqueue(cfgt.vars, create_attr_sval(T_Driftfile, "\0")); }
     break;
 
   case 207:
 
  
 #line 974 "ntp_parser.y"
     { (yyval.Set_var) = create_setvar_node((yyvsp[(1) - (4)].String), (yyvsp[(3) - (4)].String), (yyvsp[(4) - (4)].Integer)); }
     break;
 
   case 208:
 
  
 #line 976 "ntp_parser.y"
     { (yyval.Set_var) = create_setvar_node((yyvsp[(1) - (3)].String), (yyvsp[(3) - (3)].String), 0); }
     break;
 
   case 209:
 
  
 #line 981 "ntp_parser.y"
     { (yyval.Queue) = enqueue((yyvsp[(1) - (2)].Queue), (yyvsp[(2) - (2)].Attr_val)); }
     break;
 
   case 210:
 
  
 #line 982 "ntp_parser.y"
     { (yyval.Queue) = enqueue_in_new_queue((yyvsp[(1) - (1)].Attr_val)); }
     break;
 
   case 211:
 
  
 #line 986 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_ival((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Integer)); }
     break;
 
   case 212:
 
  
 #line 987 "ntp_parser.y"
     { (yyval.Attr_val) = create_attr_pval((yyvsp[(1) - (2)].Integer), (yyvsp[(2) - (2)].Address_node)); }
     break;
 
   case 213:
 
  
 #line 991 "ntp_parser.y"
     { (yyval.Queue) = enqueue((yyvsp[(1) - (2)].Queue), (yyvsp[(2) - (2)].Attr_val)); }
     break;
 
   case 214:
 
  
 #line 992 "ntp_parser.y"
     { (yyval.Queue) = enqueue_in_new_queue((yyvsp[(1) - (1)].Attr_val)); }
     break;
 
   case 215:
 
   
  #line 997 "ntp_parser.y"
      {
			char prefix = (yyvsp[(1) - (1)].String)[0];
			char *type = (yyvsp[(1) - (1)].String) + 1;
// 			char	prefix;
// 			char *	type;
  			
			if (prefix != '+' && prefix != '-' && prefix != '=') {
				yyerror("Logconfig prefix is not '+', '-' or '='\n");
			}
			else
				(yyval.Attr_val) = create_attr_sval(prefix, estrdup(type));
// 			switch ((yyvsp[(1) - (1)].String)[0]) {
// 			
// 			case '+':
// 			case '-':
// 			case '=':
// 				prefix = (yyvsp[(1) - (1)].String)[0];
// 				type = (yyvsp[(1) - (1)].String) + 1;
// 				break;
// 				
// 			default:
// 				prefix = '=';
// 				type = (yyvsp[(1) - (1)].String);
// 			}	
// 			
// 			(yyval.Attr_val) = create_attr_sval(prefix, estrdup(type));
  			YYFREE((yyvsp[(1) - (1)].String));
  		}
      break;
  
    case 216:
  
   
#line 1012 "ntp_parser.y"
// #line 1022 "ntp_parser.y"
      {
  			enqueue(cfgt.nic_rules,
  				create_nic_rule_node((yyvsp[(3) - (3)].Integer), NULL, (yyvsp[(2) - (3)].Integer)));
 		}
     break;
 
    case 217:
  
   
#line 1017 "ntp_parser.y"
// #line 1027 "ntp_parser.y"
      {
  			enqueue(cfgt.nic_rules,
  				create_nic_rule_node(0, (yyvsp[(3) - (3)].String), (yyvsp[(2) - (3)].Integer)));
 		}
     break;
 
    case 227:
  
   
#line 1048 "ntp_parser.y"
// #line 1058 "ntp_parser.y"
      { (yyval.Queue) = enqueue((yyvsp[(1) - (2)].Queue), create_ival((yyvsp[(2) - (2)].Integer))); }
      break;
  
    case 228:
  
   
#line 1049 "ntp_parser.y"
// #line 1059 "ntp_parser.y"
      { (yyval.Queue) = enqueue_in_new_queue(create_ival((yyvsp[(1) - (1)].Integer))); }
      break;
  
    case 229:
  
   
#line 1054 "ntp_parser.y"
// #line 1064 "ntp_parser.y"
      { (yyval.Queue) = enqueue((yyvsp[(1) - (2)].Queue), (yyvsp[(2) - (2)].Attr_val)); }
      break;
  
    case 230:
  
   
#line 1056 "ntp_parser.y"
// #line 1066 "ntp_parser.y"
      { (yyval.Queue) = enqueue_in_new_queue((yyvsp[(1) - (1)].Attr_val)); }
      break;
  
    case 231:
  
   
#line 1061 "ntp_parser.y"
// #line 1071 "ntp_parser.y"
      { (yyval.Attr_val) = create_attr_ival('i', (yyvsp[(1) - (1)].Integer)); }
      break;
  
    case 233:
  
   
#line 1067 "ntp_parser.y"
// #line 1077 "ntp_parser.y"
      { (yyval.Attr_val) = create_attr_shorts('-', (yyvsp[(2) - (5)].Integer), (yyvsp[(4) - (5)].Integer)); }
      break;
  
    case 234:
  
   
#line 1071 "ntp_parser.y"
// #line 1081 "ntp_parser.y"
      { (yyval.Queue) = enqueue((yyvsp[(1) - (2)].Queue), create_pval((yyvsp[(2) - (2)].String))); }
      break;
  
    case 235:
  
   
#line 1072 "ntp_parser.y"
// #line 1082 "ntp_parser.y"
      { (yyval.Queue) = enqueue_in_new_queue(create_pval((yyvsp[(1) - (1)].String))); }
      break;
  
    case 236:
  
   
#line 1076 "ntp_parser.y"
// #line 1086 "ntp_parser.y"
      { (yyval.Queue) = enqueue((yyvsp[(1) - (2)].Queue), (yyvsp[(2) - (2)].Address_node)); }
      break;
  
    case 237:
  
   
#line 1077 "ntp_parser.y"
// #line 1087 "ntp_parser.y"
      { (yyval.Queue) = enqueue_in_new_queue((yyvsp[(1) - (1)].Address_node)); }
      break;
  
    case 238:
  
   
#line 1082 "ntp_parser.y"
// #line 1092 "ntp_parser.y"
      {
  			if ((yyvsp[(1) - (1)].Integer) != 0 && (yyvsp[(1) - (1)].Integer) != 1) {
  				yyerror("Integer value is not boolean (0 or 1). Assuming 1");
 				(yyval.Integer) = 1;
 			}
 			else
 				(yyval.Integer) = (yyvsp[(1) - (1)].Integer);
 		}
     break;
 
    case 239:
  
   
#line 1090 "ntp_parser.y"
// #line 1100 "ntp_parser.y"
      { (yyval.Integer) = 1; }
      break;
  
    case 240:
  
   
#line 1091 "ntp_parser.y"
// #line 1101 "ntp_parser.y"
      { (yyval.Integer) = 0; }
      break;
  
    case 241:
  
   
#line 1095 "ntp_parser.y"
// #line 1105 "ntp_parser.y"
      { (yyval.Double) = (double)(yyvsp[(1) - (1)].Integer); }
      break;
  
    case 243:
  
   
#line 1106 "ntp_parser.y"
// #line 1116 "ntp_parser.y"
      {
  			cfgt.sim_details = create_sim_node((yyvsp[(3) - (5)].Queue), (yyvsp[(4) - (5)].Queue));
  
 			 
 			old_config_style = 1;
 		}
     break;
 
    case 244:
  
   
#line 1120 "ntp_parser.y"
// #line 1130 "ntp_parser.y"
      { old_config_style = 0; }
      break;
  
    case 245:
  
   
#line 1124 "ntp_parser.y"
// #line 1134 "ntp_parser.y"
      { (yyval.Queue) = enqueue((yyvsp[(1) - (3)].Queue), (yyvsp[(2) - (3)].Attr_val)); }
      break;
  
    case 246:
  
   
#line 1125 "ntp_parser.y"
// #line 1135 "ntp_parser.y"
      { (yyval.Queue) = enqueue_in_new_queue((yyvsp[(1) - (2)].Attr_val)); }
      break;
  
    case 247:
  
   
#line 1129 "ntp_parser.y"
// #line 1139 "ntp_parser.y"
      { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (3)].Integer), (yyvsp[(3) - (3)].Double)); }
      break;
  
    case 248:
  
   
#line 1130 "ntp_parser.y"
// #line 1140 "ntp_parser.y"
      { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (3)].Integer), (yyvsp[(3) - (3)].Double)); }
      break;
  
    case 249:
  
   
#line 1134 "ntp_parser.y"
// #line 1144 "ntp_parser.y"
      { (yyval.Queue) = enqueue((yyvsp[(1) - (2)].Queue), (yyvsp[(2) - (2)].Sim_server)); }
      break;
  
    case 250:
  
   
#line 1135 "ntp_parser.y"
// #line 1145 "ntp_parser.y"
      { (yyval.Queue) = enqueue_in_new_queue((yyvsp[(1) - (1)].Sim_server)); }
      break;
  
    case 251:
  
   
#line 1140 "ntp_parser.y"
// #line 1150 "ntp_parser.y"
      { (yyval.Sim_server) = create_sim_server((yyvsp[(1) - (5)].Address_node), (yyvsp[(3) - (5)].Double), (yyvsp[(4) - (5)].Queue)); }
      break;
  
    case 252:
  
   
#line 1144 "ntp_parser.y"
// #line 1154 "ntp_parser.y"
      { (yyval.Double) = (yyvsp[(3) - (4)].Double); }
      break;
  
    case 253:
  
   
#line 1148 "ntp_parser.y"
// #line 1158 "ntp_parser.y"
      { (yyval.Address_node) = (yyvsp[(3) - (3)].Address_node); }
      break;
  
    case 254:
  
   
#line 1152 "ntp_parser.y"
// #line 1162 "ntp_parser.y"
      { (yyval.Queue) = enqueue((yyvsp[(1) - (2)].Queue), (yyvsp[(2) - (2)].Sim_script)); }
      break;
  
    case 255:
  
   
#line 1153 "ntp_parser.y"
// #line 1163 "ntp_parser.y"
      { (yyval.Queue) = enqueue_in_new_queue((yyvsp[(1) - (1)].Sim_script)); }
      break;
  
    case 256:
  
   
#line 1158 "ntp_parser.y"
// #line 1168 "ntp_parser.y"
      { (yyval.Sim_script) = create_sim_script_info((yyvsp[(3) - (6)].Double), (yyvsp[(5) - (6)].Queue)); }
      break;
  
    case 257:
  
   
#line 1162 "ntp_parser.y"
// #line 1172 "ntp_parser.y"
      { (yyval.Queue) = enqueue((yyvsp[(1) - (3)].Queue), (yyvsp[(2) - (3)].Attr_val)); }
      break;
  
    case 258:
  
   
#line 1163 "ntp_parser.y"
// #line 1173 "ntp_parser.y"
      { (yyval.Queue) = enqueue_in_new_queue((yyvsp[(1) - (2)].Attr_val)); }
      break;
  
    case 259:
  
   
#line 1168 "ntp_parser.y"
// #line 1178 "ntp_parser.y"
      { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (3)].Integer), (yyvsp[(3) - (3)].Double)); }
      break;
  
    case 260:
  
   
#line 1170 "ntp_parser.y"
// #line 1180 "ntp_parser.y"
      { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (3)].Integer), (yyvsp[(3) - (3)].Double)); }
      break;
  
    case 261:
  
   
#line 1172 "ntp_parser.y"
// #line 1182 "ntp_parser.y"
      { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (3)].Integer), (yyvsp[(3) - (3)].Double)); }
      break;
  
    case 262:
  
   
#line 1174 "ntp_parser.y"
// #line 1184 "ntp_parser.y"
      { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (3)].Integer), (yyvsp[(3) - (3)].Double)); }
      break;
  
    case 263:
  
   
#line 1176 "ntp_parser.y"
// #line 1186 "ntp_parser.y"
      { (yyval.Attr_val) = create_attr_dval((yyvsp[(1) - (3)].Integer), (yyvsp[(3) - (3)].Double)); }
      break;
  
  
  
   
#line 3826 "ntp_parser.c"
// #line 3836 "ntp_parser.c"
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
    
   if (!yyerrstatus)
     {
       ++yynerrs;
 #if ! YYERROR_VERBOSE
       yyerror (YY_("syntax error"));
 #else
       {
 	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
 	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
 	  {
 	    YYSIZE_T yyalloc = 2 * yysize;
 	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
 	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
 	    if (yymsg != yymsgbuf)
 	      YYSTACK_FREE (yymsg);
 	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
 	    if (yymsg)
 	      yymsg_alloc = yyalloc;
 	    else
 	      {
 		yymsg = yymsgbuf;
 		yymsg_alloc = sizeof yymsgbuf;
 	      }
 	  }
 
 	if (0 < yysize && yysize <= yymsg_alloc)
 	  {
 	    (void) yysyntax_error (yymsg, yystate, yychar);
 	    yyerror (yymsg);
 	  }
 	else
 	  {
 	    yyerror (YY_("syntax error"));
 	    if (yysize != 0)
 	      goto yyexhaustedlab;
 	  }
       }
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
 		      yytoken, &yylval);
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
       if (yyn != YYPACT_NINF)
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
 		  yystos[yystate], yyvsp);
       YYPOPSTACK (1);
       yystate = *yyssp;
       YY_STACK_PRINT (yyss, yyssp);
     }
 
   *++yyvsp = yylval;
 
 
    
   YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);
 
   yystate = yyn;
   goto yynewstate;
 
 
  
 yyacceptlab:
   yyresult = 0;
   goto yyreturn;
 
  
 yyabortlab:
   yyresult = 1;
   goto yyreturn;
 
 #if !defined(yyoverflow) || YYERROR_VERBOSE
  
 yyexhaustedlab:
   yyerror (YY_("memory exhausted"));
   yyresult = 2;
    
 #endif
 
 yyreturn:
   if (yychar != YYEMPTY)
      yydestruct ("Cleanup: discarding lookahead",
 		 yytoken, &yylval);
    
   YYPOPSTACK (yylen);
   YY_STACK_PRINT (yyss, yyssp);
   while (yyssp != yyss)
     {
       yydestruct ("Cleanup: popping",
 		  yystos[*yyssp], yyvsp);
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
    
   return YYID (yyresult);
 }