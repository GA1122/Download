static js_Ast *fundec(js_State *J)
{
	js_Ast *a, *b, *c;
	a = identifier(J);
	jsP_expect(J, '(');
	b = parameters(J);
	jsP_expect(J, ')');
	c = funbody(J);
	return jsP_newnode(J, AST_FUNDEC, a, b, c, 0);
}
