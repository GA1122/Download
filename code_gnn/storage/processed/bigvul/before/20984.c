static void *m_start(struct seq_file *m, loff_t *pos)
{
	struct proc_maps_private *priv = m->private;
	unsigned long last_addr = m->version;
	struct mm_struct *mm;
	struct vm_area_struct *vma, *tail_vma = NULL;
	loff_t l = *pos;

	 
	priv->task = NULL;
	priv->tail_vma = NULL;

	 

	if (last_addr == -1UL)
		return NULL;

	priv->task = get_pid_task(priv->pid, PIDTYPE_PID);
	if (!priv->task)
		return ERR_PTR(-ESRCH);

	mm = mm_for_maps(priv->task);
	if (!mm || IS_ERR(mm))
		return mm;
	down_read(&mm->mmap_sem);

	tail_vma = get_gate_vma(priv->task->mm);
	priv->tail_vma = tail_vma;

	 
	vma = find_vma(mm, last_addr);
	if (last_addr && vma) {
		vma = vma->vm_next;
		goto out;
	}

	 
	vma = NULL;
	if ((unsigned long)l < mm->map_count) {
		vma = mm->mmap;
		while (l-- && vma)
			vma = vma->vm_next;
		goto out;
	}

	if (l != mm->map_count)
		tail_vma = NULL;  

out:
	if (vma)
		return vma;

	 
	m->version = (tail_vma != NULL)? 0: -1UL;
	up_read(&mm->mmap_sem);
	mmput(mm);
	return tail_vma;
}
