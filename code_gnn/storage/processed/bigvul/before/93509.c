	__releases(mrt_lock)
{
	read_unlock(&mrt_lock);
}
