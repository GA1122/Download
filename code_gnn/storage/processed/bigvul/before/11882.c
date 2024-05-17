static js_Ast *funexp(js_State *J)
{
	js_Ast *a, *b, *c;
	a = identifieropt(J);
	jsP_expect(J, '(');
	b = parameters(J);
	jsP_expect(J, ')');
	c = funbody(J);
	return EXP3(FUN, a, b, c);
}
