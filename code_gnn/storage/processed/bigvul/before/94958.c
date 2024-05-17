static struct dquot **ext2_get_dquots(struct inode *inode)
{
	return EXT2_I(inode)->i_dquot;
}
