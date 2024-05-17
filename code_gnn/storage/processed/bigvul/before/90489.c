static void m_cache_vma(struct seq_file *m, struct vm_area_struct *vma)
{
	if (m->count < m->size)	 
		m->version = m_next_vma(m->private, vma) ? vma->vm_end : -1UL;
}
