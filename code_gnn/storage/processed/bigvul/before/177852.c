 parse_object(JsonLexContext *lex, JsonSemAction *sem)
 {
 	 
 	json_struct_action ostart = sem->object_start;
     json_struct_action oend = sem->object_end;
     JsonTokenType tok;
  
//    check_stack_depth();
// 
     if (ostart != NULL)
         (*ostart) (sem->semstate);
  
 	 * itself. Note that we increment this after we call the semantic routine
 	 * for the object start and restore it before we call the routine for the
 	 * object end.
 	 */
 	lex->lex_level++;
 
 	 
 	lex_expect(JSON_PARSE_OBJECT_START, lex, JSON_TOKEN_OBJECT_START);
 
 	tok = lex_peek(lex);
 	switch (tok)
 	{
 		case JSON_TOKEN_STRING:
 			parse_object_field(lex, sem);
 			while (lex_accept(lex, JSON_TOKEN_COMMA, NULL))
 				parse_object_field(lex, sem);
 			break;
 		case JSON_TOKEN_OBJECT_END:
 			break;
 		default:
 			 
 			report_parse_error(JSON_PARSE_OBJECT_START, lex);
 	}
 
 	lex_expect(JSON_PARSE_OBJECT_NEXT, lex, JSON_TOKEN_OBJECT_END);
 
 	lex->lex_level--;
 
 	if (oend != NULL)
 		(*oend) (sem->semstate);
 }