 static void ctrycatchfinally(JF, js_Ast *trystm, js_Ast *catchvar, js_Ast *catchstm, js_Ast *finallystm)
 {
 	int L1, L2, L3;
 	L1 = emitjump(J, F, OP_TRY);
 	{
 		 
 		L2 = emitjump(J, F, OP_TRY);
 		{
 			 
 			cstm(J, F, finallystm);  
 			emit(J, F, OP_THROW);  
 		}
 		label(J, F, L2);
 		if (F->strict) {
 			checkfutureword(J, F, catchvar);
 			if (!strcmp(catchvar->string, "arguments"))
 				jsC_error(J, catchvar, "redefining 'arguments' is not allowed in strict mode");
 			if (!strcmp(catchvar->string, "eval"))
 				jsC_error(J, catchvar, "redefining 'eval' is not allowed in strict mode");
 		}
 		emitline(J, F, catchvar);
  		emitstring(J, F, OP_CATCH, catchvar->string);
  		cstm(J, F, catchstm);
  		emit(J, F, OP_ENDCATCH);
// 		emit(J, F, OP_ENDTRY);
  		L3 = emitjump(J, F, OP_JUMP);  
  	}
  	label(J, F, L1);
 	cstm(J, F, trystm);
 	emit(J, F, OP_ENDTRY);
 	label(J, F, L3);
 	cstm(J, F, finallystm);
 }