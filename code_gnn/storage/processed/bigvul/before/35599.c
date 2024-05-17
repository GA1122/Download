setup_syscalls_segments(struct x86_emulate_ctxt *ctxt,
			struct desc_struct *cs, struct desc_struct *ss)
{
	cs->l = 0;		 
	set_desc_base(cs, 0);	 
	cs->g = 1;		 
	set_desc_limit(cs, 0xfffff);	 
	cs->type = 0x0b;	 
	cs->s = 1;
	cs->dpl = 0;		 
	cs->p = 1;
	cs->d = 1;
	cs->avl = 0;

	set_desc_base(ss, 0);	 
	set_desc_limit(ss, 0xfffff);	 
	ss->g = 1;		 
	ss->s = 1;
	ss->type = 0x03;	 
	ss->d = 1;		 
	ss->dpl = 0;
	ss->p = 1;
	ss->l = 0;
	ss->avl = 0;
}
