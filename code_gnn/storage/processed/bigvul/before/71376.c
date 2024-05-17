static int update_refs_from_report(
	git_vector *refs,
	git_vector *push_specs,
	git_vector *push_report)
{
	git_pkt_ref *ref;
	push_spec *push_spec;
	push_status *push_status;
	size_t i, j, refs_len;
	int cmp;

	 
	if (push_specs->length != push_report->length) {
		giterr_set(GITERR_NET, "report-status: protocol error");
		return -1;
	}

	 
	git_vector_sort(push_specs);
	git_vector_sort(push_report);

	git_vector_foreach(push_specs, i, push_spec) {
		push_status = git_vector_get(push_report, i);

		 
		if (strcmp(push_spec->refspec.dst, push_status->ref)) {
			giterr_set(GITERR_NET, "report-status: protocol error");
			return -1;
		}
	}

	 
	git_vector_sort(refs);
	i = j = 0;
	refs_len = refs->length;

	 
	while (i < push_specs->length && j < refs_len) {
		push_spec = git_vector_get(push_specs, i);
		push_status = git_vector_get(push_report, i);
		ref = git_vector_get(refs, j);

		cmp = strcmp(push_spec->refspec.dst, ref->head.name);

		 
		if (cmp <= 0) i++;
		if (cmp >= 0) j++;

		 
		if (cmp < 0 &&
			!push_status->msg &&
			add_ref_from_push_spec(refs, push_spec) < 0)
			return -1;

		 
		if (cmp == 0 &&
			!push_status->msg)
			git_oid_cpy(&ref->head.oid, &push_spec->loid);
	}

	for (; i < push_specs->length; i++) {
		push_spec = git_vector_get(push_specs, i);
		push_status = git_vector_get(push_report, i);

		 
		if (!push_status->msg &&
			add_ref_from_push_spec(refs, push_spec) < 0)
			return -1;
	}

	 
	git_vector_rforeach(refs, i, ref) {
		if (git_oid_iszero(&ref->head.oid)) {
			git_vector_remove(refs, i);
			git_pkt_free((git_pkt *)ref);
		}
	}

	git_vector_sort(refs);

	return 0;
}
