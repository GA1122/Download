static js_Ast *primary(js_State *J)
{
	js_Ast *a;

	if (J->lookahead == TK_IDENTIFIER) {
		a = jsP_newstrnode(J, EXP_IDENTIFIER, J->text);
		jsP_next(J);
		return a;
	}
	if (J->lookahead == TK_STRING) {
		a = jsP_newstrnode(J, EXP_STRING, J->text);
		jsP_next(J);
		return a;
	}
	if (J->lookahead == TK_REGEXP) {
		a = jsP_newstrnode(J, EXP_REGEXP, J->text);
		a->number = J->number;
		jsP_next(J);
		return a;
	}
	if (J->lookahead == TK_NUMBER) {
		a = jsP_newnumnode(J, EXP_NUMBER, J->number);
		jsP_next(J);
		return a;
	}

	if (jsP_accept(J, TK_THIS)) return EXP0(THIS);
	if (jsP_accept(J, TK_NULL)) return EXP0(NULL);
	if (jsP_accept(J, TK_TRUE)) return EXP0(TRUE);
	if (jsP_accept(J, TK_FALSE)) return EXP0(FALSE);
	if (jsP_accept(J, '{')) { a = EXP1(OBJECT, objectliteral(J)); jsP_expect(J, '}'); return a; }
	if (jsP_accept(J, '[')) { a = EXP1(ARRAY, arrayliteral(J)); jsP_expect(J, ']'); return a; }
	if (jsP_accept(J, '(')) { a = expression(J, 0); jsP_expect(J, ')'); return a; }

	jsP_error(J, "unexpected token in expression: %s", jsY_tokenstring(J->lookahead));
}
