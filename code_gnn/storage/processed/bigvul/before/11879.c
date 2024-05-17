static js_Ast *arrayelement(js_State *J)
{
	if (J->lookahead == ',')
		return EXP0(UNDEF);
	return assignment(J, 0);
}
