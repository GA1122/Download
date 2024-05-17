DecodeSpecial(int field, char *lowtoken, int *val)
{
	int			type;
	datetkn    *tp;

	if (datecache[field] != NULL &&
		strncmp(lowtoken, datecache[field]->token, TOKMAXLEN) == 0)
		tp = datecache[field];
	else
	{
		tp = NULL;
		if (!tp)
			tp = datebsearch(lowtoken, datetktbl, szdatetktbl);
	}
	datecache[field] = tp;
	if (tp == NULL)
	{
		type = UNKNOWN_FIELD;
		*val = 0;
	}
	else
	{
		type = tp->type;
		switch (type)
		{
			case TZ:
			case DTZ:
			case DTZMOD:
				*val = FROMVAL(tp);
				break;

			default:
				*val = tp->value;
				break;
		}
	}

	return type;
}	 