int __compat_save_altstack(compat_stack_t __user *uss, unsigned long sp)
{
	struct task_struct *t = current;
	return  __put_user(ptr_to_compat((void __user *)t->sas_ss_sp), &uss->ss_sp) |
		__put_user(sas_ss_flags(sp), &uss->ss_flags) |
		__put_user(t->sas_ss_size, &uss->ss_size);
}