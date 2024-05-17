long do_shmat(int shmid, char __user *shmaddr, int shmflg, ulong *raddr,
	      unsigned long shmlba)
{
	struct shmid_kernel *shp;
	unsigned long addr;
	unsigned long size;
	struct file *file;
	int    err;
	unsigned long flags;
	unsigned long prot;
	int acc_mode;
	struct ipc_namespace *ns;
	struct shm_file_data *sfd;
	struct path path;
	fmode_t f_mode;
	unsigned long populate = 0;

	err = -EINVAL;
	if (shmid < 0)
		goto out;
	else if ((addr = (ulong)shmaddr)) {
		if (addr & (shmlba - 1)) {
			if (shmflg & SHM_RND)
				addr &= ~(shmlba - 1);	    
			else
#ifndef __ARCH_FORCE_SHMLBA
				if (addr & ~PAGE_MASK)
#endif
					goto out;
		}
		flags = MAP_SHARED | MAP_FIXED;
	} else {
		if ((shmflg & SHM_REMAP))
			goto out;

		flags = MAP_SHARED;
	}

	if (shmflg & SHM_RDONLY) {
		prot = PROT_READ;
		acc_mode = S_IRUGO;
		f_mode = FMODE_READ;
	} else {
		prot = PROT_READ | PROT_WRITE;
		acc_mode = S_IRUGO | S_IWUGO;
		f_mode = FMODE_READ | FMODE_WRITE;
	}
	if (shmflg & SHM_EXEC) {
		prot |= PROT_EXEC;
		acc_mode |= S_IXUGO;
	}

	 
	ns = current->nsproxy->ipc_ns;
	rcu_read_lock();
	shp = shm_obtain_object_check(ns, shmid);
	if (IS_ERR(shp)) {
		err = PTR_ERR(shp);
		goto out_unlock;
	}

	err = -EACCES;
	if (ipcperms(ns, &shp->shm_perm, acc_mode))
		goto out_unlock;

	err = security_shm_shmat(shp, shmaddr, shmflg);
	if (err)
		goto out_unlock;

	ipc_lock_object(&shp->shm_perm);

	 
	if (!ipc_valid_object(&shp->shm_perm)) {
		ipc_unlock_object(&shp->shm_perm);
		err = -EIDRM;
		goto out_unlock;
	}

	path = shp->shm_file->f_path;
	path_get(&path);
	shp->shm_nattch++;
	size = i_size_read(d_inode(path.dentry));
	ipc_unlock_object(&shp->shm_perm);
	rcu_read_unlock();

	err = -ENOMEM;
	sfd = kzalloc(sizeof(*sfd), GFP_KERNEL);
	if (!sfd) {
		path_put(&path);
		goto out_nattch;
	}

	file = alloc_file(&path, f_mode,
			  is_file_hugepages(shp->shm_file) ?
				&shm_file_operations_huge :
				&shm_file_operations);
	err = PTR_ERR(file);
	if (IS_ERR(file)) {
		kfree(sfd);
		path_put(&path);
		goto out_nattch;
	}

	file->private_data = sfd;
	file->f_mapping = shp->shm_file->f_mapping;
	sfd->id = shp->shm_perm.id;
	sfd->ns = get_ipc_ns(ns);
	sfd->file = shp->shm_file;
	sfd->vm_ops = NULL;

	err = security_mmap_file(file, prot, flags);
	if (err)
		goto out_fput;

	down_write(&current->mm->mmap_sem);
	if (addr && !(shmflg & SHM_REMAP)) {
		err = -EINVAL;
		if (addr + size < addr)
			goto invalid;

		if (find_vma_intersection(current->mm, addr, addr + size))
			goto invalid;
	}

	addr = do_mmap_pgoff(file, addr, size, prot, flags, 0, &populate);
	*raddr = addr;
	err = 0;
	if (IS_ERR_VALUE(addr))
		err = (long)addr;
invalid:
	up_write(&current->mm->mmap_sem);
	if (populate)
		mm_populate(addr, populate);

out_fput:
	fput(file);

out_nattch:
	down_write(&shm_ids(ns).rwsem);
	shp = shm_lock(ns, shmid);
	shp->shm_nattch--;
	if (shm_may_destroy(ns, shp))
		shm_destroy(ns, shp);
	else
		shm_unlock(shp);
	up_write(&shm_ids(ns).rwsem);
	return err;

out_unlock:
	rcu_read_unlock();
out:
	return err;
}