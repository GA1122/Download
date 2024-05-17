static inline int limiting_can_increase_treesame(const struct rev_info *revs)
{
	 
	return revs->prune && revs->dense &&
	       !revs->simplify_history &&
	       !revs->first_parent_only;
}
