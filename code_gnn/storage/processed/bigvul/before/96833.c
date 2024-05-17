void fuse_set_initialized(struct fuse_conn *fc)
{
	 
	smp_wmb();
	fc->initialized = 1;
}
