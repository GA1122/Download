summarize_posix_acl(struct posix_acl *acl, struct posix_acl_summary *pas)
{
	struct posix_acl_entry *pa, *pe;

	 
	memset(pas, 0, sizeof(*pas));
	pas->mask = 07;

	pe = acl->a_entries + acl->a_count;

	FOREACH_ACL_ENTRY(pa, acl, pe) {
		switch (pa->e_tag) {
			case ACL_USER_OBJ:
				pas->owner = pa->e_perm;
				break;
			case ACL_GROUP_OBJ:
				pas->group = pa->e_perm;
				break;
			case ACL_USER:
				pas->users |= pa->e_perm;
				break;
			case ACL_GROUP:
				pas->groups |= pa->e_perm;
				break;
			case ACL_OTHER:
				pas->other = pa->e_perm;
				break;
			case ACL_MASK:
				pas->mask = pa->e_perm;
				break;
		}
	}
	 
	pas->users &= pas->mask;
	pas->group &= pas->mask;
	pas->groups &= pas->mask;
}
