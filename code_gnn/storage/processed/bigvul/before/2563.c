IteratorConcat(JsonbIterator **it1, JsonbIterator **it2,
			   JsonbParseState **state)
{
	uint32		r1,
				r2,
				rk1,
				rk2;
	JsonbValue	v1,
				v2,
			   *res = NULL;

	r1 = rk1 = JsonbIteratorNext(it1, &v1, false);
	r2 = rk2 = JsonbIteratorNext(it2, &v2, false);

	 
	if (rk1 == WJB_BEGIN_OBJECT && rk2 == WJB_BEGIN_OBJECT)
	{
		 
		pushJsonbValue(state, r1, NULL);
		while ((r1 = JsonbIteratorNext(it1, &v1, true)) != WJB_END_OBJECT)
			pushJsonbValue(state, r1, &v1);

		 
		while ((r2 = JsonbIteratorNext(it2, &v2, true)) != 0)
			res = pushJsonbValue(state, r2, r2 != WJB_END_OBJECT ? &v2 : NULL);
	}

	 
	else if (rk1 == WJB_BEGIN_ARRAY && rk2 == WJB_BEGIN_ARRAY)
	{
		pushJsonbValue(state, r1, NULL);

		while ((r1 = JsonbIteratorNext(it1, &v1, true)) != WJB_END_ARRAY)
		{
			Assert(r1 == WJB_ELEM);
			pushJsonbValue(state, r1, &v1);
		}

		while ((r2 = JsonbIteratorNext(it2, &v2, true)) != WJB_END_ARRAY)
		{
			Assert(r2 == WJB_ELEM);
			pushJsonbValue(state, WJB_ELEM, &v2);
		}

		res = pushJsonbValue(state, WJB_END_ARRAY, NULL   );
	}
	 
	else if (((rk1 == WJB_BEGIN_ARRAY && !(*it1)->isScalar) && rk2 == WJB_BEGIN_OBJECT) ||
			 (rk1 == WJB_BEGIN_OBJECT && (rk2 == WJB_BEGIN_ARRAY && !(*it2)->isScalar)))
	{

		JsonbIterator **it_array = rk1 == WJB_BEGIN_ARRAY ? it1 : it2;
		JsonbIterator **it_object = rk1 == WJB_BEGIN_OBJECT ? it1 : it2;

		bool		prepend = (rk1 == WJB_BEGIN_OBJECT);

		pushJsonbValue(state, WJB_BEGIN_ARRAY, NULL);

		if (prepend)
		{
			pushJsonbValue(state, WJB_BEGIN_OBJECT, NULL);
			while ((r1 = JsonbIteratorNext(it_object, &v1, true)) != 0)
				pushJsonbValue(state, r1, r1 != WJB_END_OBJECT ? &v1 : NULL);

			while ((r2 = JsonbIteratorNext(it_array, &v2, true)) != 0)
				res = pushJsonbValue(state, r2, r2 != WJB_END_ARRAY ? &v2 : NULL);
		}
		else
		{
			while ((r1 = JsonbIteratorNext(it_array, &v1, true)) != WJB_END_ARRAY)
				pushJsonbValue(state, r1, &v1);

			pushJsonbValue(state, WJB_BEGIN_OBJECT, NULL);
			while ((r2 = JsonbIteratorNext(it_object, &v2, true)) != 0)
				pushJsonbValue(state, r2, r2 != WJB_END_OBJECT ? &v2 : NULL);

			res = pushJsonbValue(state, WJB_END_ARRAY, NULL);
		}
	}
	else
	{
		 
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("invalid concatenation of jsonb objects")));
	}

	return res;
}
