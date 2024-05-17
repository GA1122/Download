lt_dlexit ()
{
   
  lt_dlloader *loader;
  int	       errors   = 0;

  LT_DLMUTEX_LOCK ();
  loader = loaders;

  if (!initialized)
    {
      LT_DLMUTEX_SETERROR (LT_DLSTRERROR (SHUTDOWN));
      ++errors;
      goto done;
    }

   
  if (--initialized == 0)
    {
      int	level;

      while (handles && LT_DLIS_RESIDENT (handles))
	{
	  handles = handles->next;
	}

       
      for (level = 1; handles; ++level)
	{
	  lt_dlhandle cur = handles;
	  int saw_nonresident = 0;

	  while (cur)
	    {
	      lt_dlhandle tmp = cur;
	      cur = cur->next;
	      if (!LT_DLIS_RESIDENT (tmp))
		saw_nonresident = 1;
	      if (!LT_DLIS_RESIDENT (tmp) && tmp->info.ref_count <= level)
		{
		  if (lt_dlclose (tmp))
		    {
		      ++errors;
		    }
		   
		  if (cur)
		    {
		      for (tmp = handles; tmp; tmp = tmp->next)
			if (tmp == cur)
			  break;
		      if (! tmp)
			cur = handles;
		    }
		}
	    }
	   
	  if (!saw_nonresident)
	    break;
	}

       
      while (loader)
	{
	  lt_dlloader *next = loader->next;
	  lt_user_data data = loader->dlloader_data;
	  if (loader->dlloader_exit && loader->dlloader_exit (data))
	    {
	      ++errors;
	    }

	  LT_DLMEM_REASSIGN (loader, next);
	}
      loaders = 0;
    }

 done:
  LT_DLMUTEX_UNLOCK ();
  return errors;
}
