pushOperator(TSQueryParserState state, int8 oper)
{
	QueryOperator *tmp;

	Assert(oper == OP_NOT || oper == OP_AND || oper == OP_OR);

	tmp = (QueryOperator *) palloc0(sizeof(QueryOperator));
	tmp->type = QI_OPR;
	tmp->oper = oper;
	 

	state->polstr = lcons(tmp, state->polstr);
}