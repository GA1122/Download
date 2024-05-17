static bool match_var_range(struct mtrr_iter *iter,
			    struct kvm_mtrr_range *range)
{
	u64 start, end;

	var_mtrr_range(range, &start, &end);
	if (!(start >= iter->end || end <= iter->start)) {
		iter->range = range;

		 
		iter->partial_map |= iter->start_max < start;

		 
		iter->start_max = max(iter->start_max, end);
		return true;
	}

	return false;
}
