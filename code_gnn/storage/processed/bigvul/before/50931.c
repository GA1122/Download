void *copy_mount_options(const void __user * data)
{
	int i;
	unsigned long size;
	char *copy;

	if (!data)
		return NULL;

	copy = kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!copy)
		return ERR_PTR(-ENOMEM);

	 
	 
	size = TASK_SIZE - (unsigned long)data;
	if (size > PAGE_SIZE)
		size = PAGE_SIZE;

	i = size - exact_copy_from_user(copy, data, size);
	if (!i) {
		kfree(copy);
		return ERR_PTR(-EFAULT);
	}
	if (i != PAGE_SIZE)
		memset(copy + i, 0, PAGE_SIZE - i);
	return copy;
}
