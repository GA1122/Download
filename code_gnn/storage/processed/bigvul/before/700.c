lt_dlinit ()
{
  int	      errors   = 0;

  LT_DLMUTEX_LOCK ();

   
  if (++initialized == 1)
    {
      handles = 0;
      user_search_path = 0;  

#if HAVE_LIBDL
      errors += lt_dlloader_add (lt_dlloader_next (0), &sys_dl, "dlopen");
#endif
#if HAVE_SHL_LOAD
      errors += lt_dlloader_add (lt_dlloader_next (0), &sys_shl, "dlopen");
#endif
#ifdef __WINDOWS__
      errors += lt_dlloader_add (lt_dlloader_next (0), &sys_wll, "dlopen");
#endif
#ifdef __BEOS__
      errors += lt_dlloader_add (lt_dlloader_next (0), &sys_bedl, "dlopen");
#endif
#if HAVE_DLD
      errors += lt_dlloader_add (lt_dlloader_next (0), &sys_dld, "dld");
#endif
#if HAVE_DYLD
       errors += lt_dlloader_add (lt_dlloader_next (0), &sys_dyld, "dyld");
       errors += sys_dyld_init();
#endif
      errors += lt_dlloader_add (lt_dlloader_next (0), &presym, "dlpreload");

      if (presym_init (presym.dlloader_data))
	{
	  LT_DLMUTEX_SETERROR (LT_DLSTRERROR (INIT_LOADER));
	  ++errors;
	}
      else if (errors != 0)
	{
	  LT_DLMUTEX_SETERROR (LT_DLSTRERROR (DLOPEN_NOT_SUPPORTED));
	  ++errors;
	}
    }

  LT_DLMUTEX_UNLOCK ();

  return errors;
}
