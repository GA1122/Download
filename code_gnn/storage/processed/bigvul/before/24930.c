static int sysfs_slab_add(struct kmem_cache *s)
{
	int err;
	const char *name;
	int unmergeable;

	if (slab_state < SYSFS)
		 
		return 0;

	unmergeable = slab_unmergeable(s);
	if (unmergeable) {
		 
		sysfs_remove_link(&slab_kset->kobj, s->name);
		name = s->name;
	} else {
		 
		name = create_unique_id(s);
	}

	s->kobj.kset = slab_kset;
	err = kobject_init_and_add(&s->kobj, &slab_ktype, NULL, name);
	if (err) {
		kobject_put(&s->kobj);
		return err;
	}

	err = sysfs_create_group(&s->kobj, &slab_attr_group);
	if (err)
		return err;
	kobject_uevent(&s->kobj, KOBJ_ADD);
	if (!unmergeable) {
		 
		sysfs_slab_alias(s, s->name);
		kfree(name);
	}
	return 0;
}
