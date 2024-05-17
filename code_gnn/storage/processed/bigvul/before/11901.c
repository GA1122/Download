static js_Ast *propname(js_State *J)
{
	js_Ast *name;
	if (J->lookahead == TK_NUMBER) {
		name = jsP_newnumnode(J, EXP_NUMBER, J->number);
		jsP_next(J);
	} else if (J->lookahead == TK_STRING) {
		name = jsP_newstrnode(J, EXP_STRING, J->text);
		jsP_next(J);
	} else {
		name = identifiername(J);
	}
	return name;
}
