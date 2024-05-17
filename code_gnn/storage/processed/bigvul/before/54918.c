static void show_commit(struct commit *commit, void *data)
{
	mark_as_seen((struct object *)commit);
}
