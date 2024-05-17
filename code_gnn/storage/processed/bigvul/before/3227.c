sleep_between_retrievals (int count)
{
  static bool first_retrieval = true;

  if (first_retrieval)
    {
       
      first_retrieval = false;
      return;
    }

  if (opt.waitretry && count > 1)
    {
       
      if (count <= opt.waitretry)
        xsleep (count - 1);
      else
        xsleep (opt.waitretry);
    }
  else if (opt.wait)
    {
      if (!opt.random_wait || count > 1)
         
        xsleep (opt.wait);
      else
        {
           
          double waitsecs = (0.5 + random_float ()) * opt.wait;
          DEBUGP (("sleep_between_retrievals: avg=%f,sleep=%f\n",
                   opt.wait, waitsecs));
          xsleep (waitsecs);
        }
    }
}
