static js_Ast *jsP_newnumnode(js_State *J, enum js_AstType type, double n)
{
	js_Ast *node = jsP_newnode(J, type, 0, 0, 0, 0);
	node->number = n;
	return node;
}
