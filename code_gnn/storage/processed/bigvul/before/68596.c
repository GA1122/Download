static void shm_close(struct vm_area_struct *vma)
{
	struct file *file = vma->vm_file;
	struct shm_file_data *sfd = shm_file_data(file);
	struct shmid_kernel *shp;
	struct ipc_namespace *ns = sfd->ns;

	down_write(&shm_ids(ns).rwsem);
	 
	shp = shm_lock(ns, sfd->id);

	 
	if (WARN_ON_ONCE(IS_ERR(shp)))
		goto done;  

	shp->shm_lprid = task_tgid_vnr(current);
	shp->shm_dtim = get_seconds();
	shp->shm_nattch--;
	if (shm_may_destroy(ns, shp))
		shm_destroy(ns, shp);
	else
		shm_unlock(shp);
done:
	up_write(&shm_ids(ns).rwsem);
}
