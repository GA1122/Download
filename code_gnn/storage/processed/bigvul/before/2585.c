get_jsonb_path_all(FunctionCallInfo fcinfo, bool as_text)
{
	Jsonb	   *jb = PG_GETARG_JSONB(0);
	ArrayType  *path = PG_GETARG_ARRAYTYPE_P(1);
	Jsonb	   *res;
	Datum	   *pathtext;
	bool	   *pathnulls;
	int			npath;
	int			i;
	bool		have_object = false,
				have_array = false;
	JsonbValue *jbvp = NULL;
	JsonbValue	tv;
	JsonbContainer *container;

	 
	if (array_contains_nulls(path))
		PG_RETURN_NULL();

	deconstruct_array(path, TEXTOID, -1, false, 'i',
					  &pathtext, &pathnulls, &npath);

	 
	container = &jb->root;

	if (JB_ROOT_IS_OBJECT(jb))
		have_object = true;
	else if (JB_ROOT_IS_ARRAY(jb) && !JB_ROOT_IS_SCALAR(jb))
		have_array = true;
	else
	{
		Assert(JB_ROOT_IS_ARRAY(jb) && JB_ROOT_IS_SCALAR(jb));
		 
		if (npath <= 0)
			jbvp = getIthJsonbValueFromContainer(container, 0);
	}

	 
	if (npath <= 0 && jbvp == NULL)
	{
		if (as_text)
		{
			PG_RETURN_TEXT_P(cstring_to_text(JsonbToCString(NULL,
															container,
															VARSIZE(jb))));
		}
		else
		{
			 
			PG_RETURN_JSONB(jb);
		}
	}

	for (i = 0; i < npath; i++)
	{
		if (have_object)
		{
			jbvp = findJsonbValueFromContainerLen(container,
												  JB_FOBJECT,
												  VARDATA_ANY(pathtext[i]),
											 VARSIZE_ANY_EXHDR(pathtext[i]));
		}
		else if (have_array)
		{
			long		lindex;
			uint32		index;
			char	   *indextext = TextDatumGetCString(pathtext[i]);
			char	   *endptr;

			errno = 0;
			lindex = strtol(indextext, &endptr, 10);
			if (endptr == indextext || *endptr != '\0' || errno != 0 ||
				lindex > INT_MAX || lindex < INT_MIN)
				PG_RETURN_NULL();

			if (lindex >= 0)
			{
				index = (uint32) lindex;
			}
			else
			{
				 
				uint32		nelements;

				 
				if ((container->header & JB_FARRAY) == 0)
					elog(ERROR, "not a jsonb array");

				nelements = container->header & JB_CMASK;

				if (-lindex > nelements)
					PG_RETURN_NULL();
				else
					index = nelements + lindex;
			}

			jbvp = getIthJsonbValueFromContainer(container, index);
		}
		else
		{
			 
			PG_RETURN_NULL();
		}

		if (jbvp == NULL)
			PG_RETURN_NULL();
		else if (i == npath - 1)
			break;

		if (jbvp->type == jbvBinary)
		{
			JsonbIterator *it = JsonbIteratorInit((JsonbContainer *) jbvp->val.binary.data);
			int			r;

			r = JsonbIteratorNext(&it, &tv, true);
			container = (JsonbContainer *) jbvp->val.binary.data;
			have_object = r == WJB_BEGIN_OBJECT;
			have_array = r == WJB_BEGIN_ARRAY;
		}
		else
		{
			have_object = jbvp->type == jbvObject;
			have_array = jbvp->type == jbvArray;
		}
	}

	if (as_text)
	{
		 
		if (jbvp->type == jbvString)
			PG_RETURN_TEXT_P(cstring_to_text_with_len(jbvp->val.string.val,
													  jbvp->val.string.len));
		if (jbvp->type == jbvNull)
			PG_RETURN_NULL();
	}

	res = JsonbValueToJsonb(jbvp);

	if (as_text)
	{
		PG_RETURN_TEXT_P(cstring_to_text(JsonbToCString(NULL,
														&res->root,
														VARSIZE(res))));
	}
	else
	{
		 
		PG_RETURN_JSONB(res);
	}
}
