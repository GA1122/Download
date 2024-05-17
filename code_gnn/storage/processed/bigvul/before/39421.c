static void reset_interrupt(void)
{
	debugt(__func__, "");
	result();		 
	if (FDCS->reset) {
		pr_info("reset set in interrupt, calling %pf\n", cont->error);
		cont->error();	 
	}
	cont->redo();
}
