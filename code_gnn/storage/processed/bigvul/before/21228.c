static inline int follow_pte(struct mm_struct *mm, unsigned long address,
			     pte_t **ptepp, spinlock_t **ptlp)
{
	int res;

	 
	(void) __cond_lock(*ptlp,
			   !(res = __follow_pte(mm, address, ptepp, ptlp)));
	return res;
}