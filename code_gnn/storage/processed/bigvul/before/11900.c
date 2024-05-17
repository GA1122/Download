static js_Ast *propassign(js_State *J)
{
	js_Ast *name, *value, *arg, *body;

	name = propname(J);

	if (J->lookahead != ':' && name->type == AST_IDENTIFIER) {
		if (!strcmp(name->string, "get")) {
			name = propname(J);
			jsP_expect(J, '(');
			jsP_expect(J, ')');
			body = funbody(J);
			return EXP3(PROP_GET, name, NULL, body);
		}
		if (!strcmp(name->string, "set")) {
			name = propname(J);
			jsP_expect(J, '(');
			arg = identifier(J);
			jsP_expect(J, ')');
			body = funbody(J);
			return EXP3(PROP_SET, name, LIST(arg), body);
		}
	}

	jsP_expect(J, ':');
	value = assignment(J, 0);
	return EXP2(PROP_VAL, name, value);
}
