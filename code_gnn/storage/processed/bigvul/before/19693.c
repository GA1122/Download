static int hugetlb_acct_memory(struct hstate *h, long delta)
{
	int ret = -ENOMEM;

	spin_lock(&hugetlb_lock);
	 
	if (delta > 0) {
		if (gather_surplus_pages(h, delta) < 0)
			goto out;

		if (delta > cpuset_mems_nr(h->free_huge_pages_node)) {
			return_unused_surplus_pages(h, delta);
			goto out;
		}
	}

	ret = 0;
	if (delta < 0)
		return_unused_surplus_pages(h, (unsigned long) -delta);

out:
	spin_unlock(&hugetlb_lock);
	return ret;
}
