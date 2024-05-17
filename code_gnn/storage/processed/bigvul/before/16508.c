param_all(void)
{
	ExtArray<ParamValue> *pvs = NULL;
	MyString filename;
	int line_number;
	MyString str;
	HASHITER it = hash_iter_begin( ConfigTab, TABLESIZE );
	char *name = NULL;
	char *value = NULL;
	int i;
	ParamValue *sort_array = NULL;

	pvs = new ExtArray<ParamValue>;
	ASSERT(pvs);

	i = 0;
	while( ! hash_iter_done(it) ) {
		name = hash_iter_key(it);
		value = hash_iter_value(it);
		param_get_location(name, filename, line_number);

		(*pvs)[i].name = name;
		(*pvs)[i].value = value;
		(*pvs)[i].filename = filename;
		(*pvs)[i].lnum = line_number;
		(*pvs)[i].source = "Local Config File";

		i++;

		hash_iter_next(it);
	}
	hash_iter_delete(&it);



	sort_array = new ParamValue[pvs->getlast() + 1];
	ASSERT(sort_array);

	for (i = 0; i < pvs->getlast() + 1; i++) {
		sort_array[i] = (*pvs)[i];
	}

	qsort(sort_array, pvs->getlast() + 1, sizeof(ParamValue),
		ParamValueNameAscendingSort);

	for (i = 0; i < pvs->getlast() + 1; i++) {
		(*pvs)[i] = sort_array[i];
	}
	
	delete [] sort_array;

	return pvs;
}
