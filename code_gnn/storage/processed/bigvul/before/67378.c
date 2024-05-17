static void swap_names(struct dentry *dentry, struct dentry *target)
{
	if (unlikely(dname_external(target))) {
		if (unlikely(dname_external(dentry))) {
			 
			swap(target->d_name.name, dentry->d_name.name);
		} else {
			 
			memcpy(target->d_iname, dentry->d_name.name,
					dentry->d_name.len + 1);
			dentry->d_name.name = target->d_name.name;
			target->d_name.name = target->d_iname;
		}
	} else {
		if (unlikely(dname_external(dentry))) {
			 
			memcpy(dentry->d_iname, target->d_name.name,
					target->d_name.len + 1);
			target->d_name.name = dentry->d_name.name;
			dentry->d_name.name = dentry->d_iname;
		} else {
			 
			unsigned int i;
			BUILD_BUG_ON(!IS_ALIGNED(DNAME_INLINE_LEN, sizeof(long)));
			kmemcheck_mark_initialized(dentry->d_iname, DNAME_INLINE_LEN);
			kmemcheck_mark_initialized(target->d_iname, DNAME_INLINE_LEN);
			for (i = 0; i < DNAME_INLINE_LEN / sizeof(long); i++) {
				swap(((long *) &dentry->d_iname)[i],
				     ((long *) &target->d_iname)[i]);
			}
		}
	}
	swap(dentry->d_name.hash_len, target->d_name.hash_len);
}