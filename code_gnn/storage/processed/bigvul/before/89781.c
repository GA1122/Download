prctl_caps (uint32_t *caps, bool do_cap_bounding, bool do_set_ambient)
{
  unsigned long cap;

   
  for (cap = 0; cap <= CAP_LAST_CAP; cap++)
    {
      bool keep = FALSE;
      if (cap < 32)
        {
          if (CAP_TO_MASK_0 (cap) & caps[0])
            keep = TRUE;
        }
      else
        {
          if (CAP_TO_MASK_1 (cap) & caps[1])
            keep = TRUE;
        }

      if (keep && do_set_ambient)
        {
#ifdef PR_CAP_AMBIENT
          int res = prctl (PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, cap, 0, 0);
          if (res == -1 && !(errno == EINVAL || errno == EPERM))
            die_with_error ("Adding ambient capability %ld", cap);
#else
           
#endif
        }

      if (!keep && do_cap_bounding)
        {
          int res = prctl (PR_CAPBSET_DROP, cap, 0, 0, 0);
          if (res == -1 && !(errno == EINVAL || errno == EPERM))
            die_with_error ("Dropping capability %ld from bounds", cap);
        }
    }
}
