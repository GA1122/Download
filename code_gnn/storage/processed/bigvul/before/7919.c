static void ctryfinally(JF, js_Ast *trystm, js_Ast *finallystm)
{
	int L1;
	L1 = emitjump(J, F, OP_TRY);
	{
		 
		cstm(J, F, finallystm);  
		emit(J, F, OP_THROW);  
	}
	label(J, F, L1);
	cstm(J, F, trystm);
	emit(J, F, OP_ENDTRY);
	cstm(J, F, finallystm);
}
