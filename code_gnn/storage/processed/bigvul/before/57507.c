static ext4_fsblk_t ext4_find_goal(struct inode *inode, ext4_lblk_t block,
				   Indirect *partial)
{
	ext4_fsblk_t goal;

	 

	goal = ext4_find_near(inode, partial);
	goal = goal & EXT4_MAX_BLOCK_FILE_PHYS;
	return goal;
}