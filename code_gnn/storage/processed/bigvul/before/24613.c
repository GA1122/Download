static u64 fuse_get_unique(struct fuse_conn *fc)
{
	fc->reqctr++;
	 
	if (fc->reqctr == 0)
		fc->reqctr = 1;

	return fc->reqctr;
}
