void move_hugetlb_state(struct page *oldpage, struct page *newpage, int reason)
{
	struct hstate *h = page_hstate(oldpage);

	hugetlb_cgroup_migrate(oldpage, newpage);
	set_page_owner_migrate_reason(newpage, reason);

	 
	if (PageHugeTemporary(newpage)) {
		int old_nid = page_to_nid(oldpage);
		int new_nid = page_to_nid(newpage);

		SetPageHugeTemporary(oldpage);
		ClearPageHugeTemporary(newpage);

		spin_lock(&hugetlb_lock);
		if (h->surplus_huge_pages_node[old_nid]) {
			h->surplus_huge_pages_node[old_nid]--;
			h->surplus_huge_pages_node[new_nid]++;
		}
		spin_unlock(&hugetlb_lock);
	}
}
