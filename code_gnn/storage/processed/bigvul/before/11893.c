static js_Ast *jsP_newstrnode(js_State *J, enum js_AstType type, const char *s)
{
	js_Ast *node = jsP_newnode(J, type, 0, 0, 0, 0);
	node->string = s;
	return node;
}
