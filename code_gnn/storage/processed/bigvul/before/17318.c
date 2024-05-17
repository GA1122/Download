cpl_reap ()
{
  struct cpelement *p, *next, *nh, *nt;

   
  nh = nt = next = (struct cpelement *)0;
  for (p = coproc_list.head; p; p = next)
    {
      next = p->next;
      if (p->coproc->c_flags & COPROC_DEAD)
	{
	  coproc_list.ncoproc--;	 

#if defined (DEBUG)
	  itrace("cpl_reap: deleting %d", p->coproc->c_pid);
#endif

	  coproc_dispose (p->coproc);
	  cpe_dispose (p);
	}
      else if (nh == 0)
	nh = nt = p;
      else
	{
	  nt->next = p;
	  nt = nt->next;
	}
    }

  if (coproc_list.ncoproc == 0)
    coproc_list.head = coproc_list.tail = 0;
  else
    {
      if (nt)
        nt->next = 0;
      coproc_list.head = nh;
      coproc_list.tail = nt;
      if (coproc_list.ncoproc == 1)
	coproc_list.tail = coproc_list.head;		   
    }
}
