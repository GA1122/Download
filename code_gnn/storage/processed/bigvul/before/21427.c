static int wait_for_discard(void *word)
{
	schedule();
	return 0;
}
