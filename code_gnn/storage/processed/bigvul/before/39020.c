findoprnd_recurse(QueryItem *ptr, uint32 *pos, int nnodes)
{
	 
	check_stack_depth();

	if (*pos >= nnodes)
		elog(ERROR, "malformed tsquery: operand not found");

	if (ptr[*pos].type == QI_VAL ||
		ptr[*pos].type == QI_VALSTOP)	 
	{
		(*pos)++;
	}
	else
	{
		Assert(ptr[*pos].type == QI_OPR);

		if (ptr[*pos].qoperator.oper == OP_NOT)
		{
			ptr[*pos].qoperator.left = 1;
			(*pos)++;
			findoprnd_recurse(ptr, pos, nnodes);
		}
		else
		{
			QueryOperator *curitem = &ptr[*pos].qoperator;
			int			tmp = *pos;

			Assert(curitem->oper == OP_AND || curitem->oper == OP_OR);

			(*pos)++;
			findoprnd_recurse(ptr, pos, nnodes);
			curitem->left = *pos - tmp;
			findoprnd_recurse(ptr, pos, nnodes);
		}
	}
}