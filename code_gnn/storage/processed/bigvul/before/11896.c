static js_Ast *newexp(js_State *J)
{
	js_Ast *a, *b;

	if (jsP_accept(J, TK_NEW)) {
		a = memberexp(J);
		if (jsP_accept(J, '(')) {
			b = arguments(J);
			jsP_expect(J, ')');
			return EXP2(NEW, a, b);
		}
		return EXP1(NEW, a);
	}

	if (jsP_accept(J, TK_FUNCTION))
		return funexp(J);

	return primary(J);
}
