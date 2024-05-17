static int __init hugepage_init(void)
{
	int err;
#ifdef CONFIG_SYSFS
	static struct kobject *hugepage_kobj;
#endif

	err = -EINVAL;
	if (!has_transparent_hugepage()) {
		transparent_hugepage_flags = 0;
		goto out;
	}

#ifdef CONFIG_SYSFS
	err = -ENOMEM;
	hugepage_kobj = kobject_create_and_add("transparent_hugepage", mm_kobj);
	if (unlikely(!hugepage_kobj)) {
		printk(KERN_ERR "hugepage: failed kobject create\n");
		goto out;
	}

	err = sysfs_create_group(hugepage_kobj, &hugepage_attr_group);
	if (err) {
		printk(KERN_ERR "hugepage: failed register hugeage group\n");
		goto out;
	}

	err = sysfs_create_group(hugepage_kobj, &khugepaged_attr_group);
	if (err) {
		printk(KERN_ERR "hugepage: failed register hugeage group\n");
		goto out;
	}
#endif

	err = khugepaged_slab_init();
	if (err)
		goto out;

	err = mm_slots_hash_init();
	if (err) {
		khugepaged_slab_free();
		goto out;
	}

	 
	if (totalram_pages < (512 << (20 - PAGE_SHIFT)))
		transparent_hugepage_flags = 0;

	start_khugepaged();

	set_recommended_min_free_kbytes();

out:
	return err;
}