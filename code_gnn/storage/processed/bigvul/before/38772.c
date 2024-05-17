contains_required_value(ITEM *curitem)
{
	 
	check_stack_depth();

	if (curitem->type == VAL)
		return true;
	else if (curitem->val == (int32) '!')
	{
		 
		return false;
	}
	else if (curitem->val == (int32) '&')
	{
		 
		if (contains_required_value(curitem + curitem->left))
			return true;
		else
			return contains_required_value(curitem - 1);
	}
	else
	{							 
		 
		if (contains_required_value(curitem + curitem->left))
			return contains_required_value(curitem - 1);
		else
			return false;
	}
}
