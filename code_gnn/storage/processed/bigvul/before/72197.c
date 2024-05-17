mm_sync_list(struct mmtree *oldtree, struct mmtree *newtree,
    struct mm_master *mm, struct mm_master *mmold)
{
	struct mm_master *mmalloc = mm->mmalloc;
	struct mm_share *mms, *new;

	 
	RB_FOREACH(mms, mmtree, oldtree) {
		 
		mm_memvalid(mmold, mms, sizeof(struct mm_share));
		mm_memvalid(mm, mms->address, mms->size);

		new = mm_xmalloc(mmalloc, sizeof(struct mm_share));
		memcpy(new, mms, sizeof(struct mm_share));
		RB_INSERT(mmtree, newtree, new);
	}
}