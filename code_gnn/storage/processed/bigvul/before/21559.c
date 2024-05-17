static int prctl_set_mm_exe_file(struct mm_struct *mm, unsigned int fd)
{
	struct fd exe;
	struct dentry *dentry;
	int err;

	exe = fdget(fd);
	if (!exe.file)
		return -EBADF;

	dentry = exe.file->f_path.dentry;

	 
	err = -EACCES;
	if (!S_ISREG(dentry->d_inode->i_mode)	||
	    exe.file->f_path.mnt->mnt_flags & MNT_NOEXEC)
		goto exit;

	err = inode_permission(dentry->d_inode, MAY_EXEC);
	if (err)
		goto exit;

	down_write(&mm->mmap_sem);

	 
	err = -EBUSY;
	if (mm->exe_file) {
		struct vm_area_struct *vma;

		for (vma = mm->mmap; vma; vma = vma->vm_next)
			if (vma->vm_file &&
			    path_equal(&vma->vm_file->f_path,
				       &mm->exe_file->f_path))
				goto exit_unlock;
	}

	 
	err = -EPERM;
	if (test_and_set_bit(MMF_EXE_FILE_CHANGED, &mm->flags))
		goto exit_unlock;

	err = 0;
	set_mm_exe_file(mm, exe.file);	 
exit_unlock:
	up_write(&mm->mmap_sem);

exit:
	fdput(exe);
	return err;
}