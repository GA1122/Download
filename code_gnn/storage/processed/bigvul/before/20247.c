static void hugetlb_vm_op_open(struct vm_area_struct *vma)
{
	struct resv_map *reservations = vma_resv_map(vma);

	 
	if (reservations)
		kref_get(&reservations->refs);
}
