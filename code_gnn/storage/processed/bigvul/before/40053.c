DecodeUnits(int field, char *lowtoken, int *val)
{
	int			type;
	datetkn    *tp;

	if (deltacache[field] != NULL &&
		strncmp(lowtoken, deltacache[field]->token, TOKMAXLEN) == 0)
		tp = deltacache[field];
	else
		tp = datebsearch(lowtoken, deltatktbl, szdeltatktbl);
	deltacache[field] = tp;
	if (tp == NULL)
	{
		type = UNKNOWN_FIELD;
		*val = 0;
	}
	else
	{
		type = tp->type;
		if (type == TZ || type == DTZ)
			*val = FROMVAL(tp);
		else
			*val = tp->value;
	}

	return type;
}	 