bool ldb_dn_remove_child_components(struct ldb_dn *dn, unsigned int num)
{
	unsigned int i, j;

	if ( ! ldb_dn_validate(dn)) {
		return false;
	}

	if (dn->comp_num < num) {
		return false;
	}

	for (i = 0, j = num; j < dn->comp_num; i++, j++) {
		if (i < num) {
			LDB_FREE(dn->components[i].name);
			LDB_FREE(dn->components[i].value.data);
			LDB_FREE(dn->components[i].cf_name);
			LDB_FREE(dn->components[i].cf_value.data);
		}
		dn->components[i] = dn->components[j];
	}

	dn->comp_num -= num;

	if (dn->valid_case) {
		for (i = 0; i < dn->comp_num; i++) {
			LDB_FREE(dn->components[i].cf_name);
			LDB_FREE(dn->components[i].cf_value.data);
		}
		dn->valid_case = false;
	}

	LDB_FREE(dn->casefold);
	LDB_FREE(dn->linearized);

	 
	LDB_FREE(dn->ext_linearized);
	LDB_FREE(dn->ext_components);
	dn->ext_comp_num = 0;

	return true;
}