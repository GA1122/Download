void lock_two_nondirectories(struct inode *inode1, struct inode *inode2)
{
	if (inode1 > inode2)
		swap(inode1, inode2);

	if (inode1 && !S_ISDIR(inode1->i_mode))
		mutex_lock(&inode1->i_mutex);
	if (inode2 && !S_ISDIR(inode2->i_mode) && inode2 != inode1)
		mutex_lock_nested(&inode2->i_mutex, I_MUTEX_NONDIR2);
}