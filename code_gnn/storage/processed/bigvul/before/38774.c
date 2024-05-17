execute(ITEM *curitem, void *checkval, bool calcnot,
		bool (*chkcond) (void *checkval, ITEM *item))
{
	 
	check_stack_depth();

	if (curitem->type == VAL)
		return (*chkcond) (checkval, curitem);
	else if (curitem->val == (int32) '!')
	{
		return (calcnot) ?
			((execute(curitem - 1, checkval, calcnot, chkcond)) ? false : true)
			: true;
	}
	else if (curitem->val == (int32) '&')
	{
		if (execute(curitem + curitem->left, checkval, calcnot, chkcond))
			return execute(curitem - 1, checkval, calcnot, chkcond);
		else
			return false;
	}
	else
	{							 
		if (execute(curitem + curitem->left, checkval, calcnot, chkcond))
			return true;
		else
			return execute(curitem - 1, checkval, calcnot, chkcond);
	}
}
