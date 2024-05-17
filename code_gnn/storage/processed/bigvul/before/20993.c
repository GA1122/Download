static int show_map(struct seq_file *m, void *v)
{
	struct vm_area_struct *vma = v;
	struct proc_maps_private *priv = m->private;
	struct task_struct *task = priv->task;

	show_map_vma(m, vma);

	if (m->count < m->size)   
		m->version = (vma != get_gate_vma(task->mm))
			? vma->vm_start : 0;
	return 0;
}