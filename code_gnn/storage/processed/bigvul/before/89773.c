drop_all_caps (bool keep_requested_caps)
{
  struct __user_cap_header_struct hdr = { _LINUX_CAPABILITY_VERSION_3, 0 };
  struct __user_cap_data_struct data[2] = { { 0 } };

  if (keep_requested_caps)
    {
       
      if (!opt_cap_add_or_drop_used && real_uid == 0)
        {
          assert (!is_privileged);
          return;
        }
      data[0].effective = requested_caps[0];
      data[0].permitted = requested_caps[0];
      data[0].inheritable = requested_caps[0];
      data[1].effective = requested_caps[1];
      data[1].permitted = requested_caps[1];
      data[1].inheritable = requested_caps[1];
    }

  if (capset (&hdr, data) < 0)
    {
       
      if (errno == EPERM && real_uid == 0 && !is_privileged)
        return;
      else
        die_with_error ("capset failed");
    }
}
