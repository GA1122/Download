static void set_changed_flag(struct Context *ctx, struct Header *h,
                             int local_changes, int *server_changes, int flag_name,
                             int old_hd_flag, int new_hd_flag, int h_flag)
{
   
  if ((old_hd_flag == new_hd_flag) && local_changes)
    return;

  if (new_hd_flag == h_flag)
    return;

  if (server_changes)
    *server_changes = 1;

   
  if (!local_changes)
    mutt_set_flag(ctx, h, flag_name, new_hd_flag);
}
