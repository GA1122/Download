static int rev_compare_tree(struct rev_info *revs,
			    struct commit *parent, struct commit *commit)
{
	struct tree *t1 = parent->tree;
	struct tree *t2 = commit->tree;

	if (!t1)
		return REV_TREE_NEW;
	if (!t2)
		return REV_TREE_OLD;

	if (revs->simplify_by_decoration) {
		 
		if (get_name_decoration(&commit->object))
			return REV_TREE_DIFFERENT;
		 
		if (!revs->prune_data.nr)
			return REV_TREE_SAME;
	}

	tree_difference = REV_TREE_SAME;
	DIFF_OPT_CLR(&revs->pruning, HAS_CHANGES);
	if (diff_tree_sha1(t1->object.oid.hash, t2->object.oid.hash, "",
			   &revs->pruning) < 0)
		return REV_TREE_DIFFERENT;
	return tree_difference;
}
