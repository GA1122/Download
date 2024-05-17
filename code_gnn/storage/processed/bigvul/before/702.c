presym_free_symlists ()
{
  lt_dlsymlists_t *lists;

  LT_DLMUTEX_LOCK ();

  lists = preloaded_symbols;
  while (lists)
    {
      lt_dlsymlists_t	*tmp = lists;

      lists = lists->next;
      LT_DLFREE (tmp);
    }
  preloaded_symbols = 0;

  LT_DLMUTEX_UNLOCK ();

  return 0;
}
