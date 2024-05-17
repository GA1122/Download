void put_fs_struct(struct fs_struct *fs)
{
	 
	if (atomic_dec_and_test(&fs->count)) {
		path_put(&fs->root);
		path_put(&fs->pwd);
		kmem_cache_free(fs_cachep, fs);
	}
}
