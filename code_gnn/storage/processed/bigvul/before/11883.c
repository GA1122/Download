static js_Ast *funstm(js_State *J)
{
	js_Ast *a, *b, *c;
	a = identifier(J);
	jsP_expect(J, '(');
	b = parameters(J);
	jsP_expect(J, ')');
	c = funbody(J);
	 
	return STM1(VAR, LIST(EXP2(VAR, a, EXP3(FUN, a, b, c))));
}
