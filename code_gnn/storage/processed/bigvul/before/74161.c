create_setvar_node(
	char *	var,
	char *	val,
	int	isdefault
	)
{
	setvar_node *	my_node;
	char *		pch;

	 
	pch = strchr(var, '=');
	if (NULL != pch)
		*pch = '\0';

	 
	my_node = emalloc_zero(sizeof(*my_node));
	my_node->var = var;
	my_node->val = val;
	my_node->isdefault = isdefault;

	return my_node;
}
