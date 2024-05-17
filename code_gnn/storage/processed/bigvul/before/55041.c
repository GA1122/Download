enum commit_action simplify_commit(struct rev_info *revs, struct commit *commit)
{
	enum commit_action action = get_commit_action(revs, commit);

	if (action == commit_show &&
	    !revs->show_all &&
	    revs->prune && revs->dense && want_ancestry(revs)) {
		 
		if (revs->full_diff)
			save_parents(revs, commit);
		if (rewrite_parents(revs, commit, rewrite_one) < 0)
			return commit_error;
	}
	return action;
}
