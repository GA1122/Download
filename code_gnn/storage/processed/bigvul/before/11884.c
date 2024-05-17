static js_Ast *identifier(js_State *J)
{
	js_Ast *a;
	if (J->lookahead == TK_IDENTIFIER) {
		a = jsP_newstrnode(J, AST_IDENTIFIER, J->text);
		jsP_next(J);
		return a;
	}
	jsP_error(J, "unexpected token: %s (expected identifier)", jsY_tokenstring(J->lookahead));
}
