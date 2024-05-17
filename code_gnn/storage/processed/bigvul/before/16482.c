static int ParamValueNameAscendingSort(const void *l, const void *r)
{
	const ParamValue *left = (const ParamValue*)l;
	const ParamValue *right = (const ParamValue*)r;

	if (left->name < right->name) {
		return -1;
	}

	if (left->name > right->name) {
		return 1;
	}

	return 0;
}
