int re_yylex_destroy  (yyscan_t yyscanner)
{
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;

     
	while(YY_CURRENT_BUFFER){
		re_yy_delete_buffer(YY_CURRENT_BUFFER ,yyscanner );
		YY_CURRENT_BUFFER_LVALUE = NULL;
		re_yypop_buffer_state(yyscanner);
	}

	 
	re_yyfree(yyg->yy_buffer_stack ,yyscanner);
	yyg->yy_buffer_stack = NULL;

     
        re_yyfree(yyg->yy_start_stack ,yyscanner );
        yyg->yy_start_stack = NULL;

     
    yy_init_globals( yyscanner);

     
    re_yyfree ( yyscanner , yyscanner );
    yyscanner = NULL;
    return 0;
}
