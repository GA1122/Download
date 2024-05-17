 hstore_from_record(PG_FUNCTION_ARGS)
 {
 	HeapTupleHeader rec;
 	int32		buflen;
 	HStore	   *out;
 	Pairs	   *pairs;
 	Oid			tupType;
 	int32		tupTypmod;
 	TupleDesc	tupdesc;
 	HeapTupleData tuple;
 	RecordIOData *my_extra;
 	int			ncolumns;
 	int			i,
 				j;
 	Datum	   *values;
 	bool	   *nulls;
 
 	if (PG_ARGISNULL(0))
 	{
 		Oid			argtype = get_fn_expr_argtype(fcinfo->flinfo, 0);
 
 		 
 		tupType = argtype;
 		tupTypmod = -1;
 
 		rec = NULL;
 	}
 	else
 	{
 		rec = PG_GETARG_HEAPTUPLEHEADER(0);
 
 		 
 		tupType = HeapTupleHeaderGetTypeId(rec);
 		tupTypmod = HeapTupleHeaderGetTypMod(rec);
 	}
 
 	tupdesc = lookup_rowtype_tupdesc(tupType, tupTypmod);
 	ncolumns = tupdesc->natts;
 
 	 
 	my_extra = (RecordIOData *) fcinfo->flinfo->fn_extra;
 	if (my_extra == NULL ||
 		my_extra->ncolumns != ncolumns)
 	{
 		fcinfo->flinfo->fn_extra =
 			MemoryContextAlloc(fcinfo->flinfo->fn_mcxt,
 							   sizeof(RecordIOData) - sizeof(ColumnIOData)
 							   + ncolumns * sizeof(ColumnIOData));
 		my_extra = (RecordIOData *) fcinfo->flinfo->fn_extra;
 		my_extra->record_type = InvalidOid;
 		my_extra->record_typmod = 0;
 	}
 
 	if (my_extra->record_type != tupType ||
 		my_extra->record_typmod != tupTypmod)
 	{
 		MemSet(my_extra, 0,
 			   sizeof(RecordIOData) - sizeof(ColumnIOData)
 			   + ncolumns * sizeof(ColumnIOData));
 		my_extra->record_type = tupType;
 		my_extra->record_typmod = tupTypmod;
  		my_extra->ncolumns = ncolumns;
  	}
  
// 	Assert(ncolumns <= MaxTupleAttributeNumber);		 
  	pairs = palloc(ncolumns * sizeof(Pairs));
  
  	if (rec)
 	{
 		 
 		tuple.t_len = HeapTupleHeaderGetDatumLength(rec);
 		ItemPointerSetInvalid(&(tuple.t_self));
 		tuple.t_tableOid = InvalidOid;
 		tuple.t_data = rec;
 
 		values = (Datum *) palloc(ncolumns * sizeof(Datum));
 		nulls = (bool *) palloc(ncolumns * sizeof(bool));
 
 		 
 		heap_deform_tuple(&tuple, tupdesc, values, nulls);
 	}
 	else
 	{
 		values = NULL;
 		nulls = NULL;
 	}
 
 	for (i = 0, j = 0; i < ncolumns; ++i)
 	{
 		ColumnIOData *column_info = &my_extra->columns[i];
 		Oid			column_type = tupdesc->attrs[i]->atttypid;
 		char	   *value;
 
 		 
 		if (tupdesc->attrs[i]->attisdropped)
 			continue;
 
 		pairs[j].key = NameStr(tupdesc->attrs[i]->attname);
 		pairs[j].keylen = hstoreCheckKeyLen(strlen(NameStr(tupdesc->attrs[i]->attname)));
 
 		if (!nulls || nulls[i])
 		{
 			pairs[j].val = NULL;
 			pairs[j].vallen = 4;
 			pairs[j].isnull = true;
 			pairs[j].needfree = false;
 			++j;
 			continue;
 		}
 
 		 
 		if (column_info->column_type != column_type)
 		{
 			bool		typIsVarlena;
 
 			getTypeOutputInfo(column_type,
 							  &column_info->typiofunc,
 							  &typIsVarlena);
 			fmgr_info_cxt(column_info->typiofunc, &column_info->proc,
 						  fcinfo->flinfo->fn_mcxt);
 			column_info->column_type = column_type;
 		}
 
 		value = OutputFunctionCall(&column_info->proc, values[i]);
 
 		pairs[j].val = value;
 		pairs[j].vallen = hstoreCheckValLen(strlen(value));
 		pairs[j].isnull = false;
 		pairs[j].needfree = false;
 		++j;
 	}
 
 	ncolumns = hstoreUniquePairs(pairs, j, &buflen);
 
 	out = hstorePairs(pairs, ncolumns, buflen);
 
 	ReleaseTupleDesc(tupdesc);
 
 	PG_RETURN_POINTER(out);
 }