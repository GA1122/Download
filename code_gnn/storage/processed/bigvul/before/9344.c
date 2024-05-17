apply_nonancestor_delayed_set_stat (char const *file_name, bool after_links)
{
  size_t file_name_len = strlen (file_name);
  bool check_for_renamed_directories = 0;

  while (delayed_set_stat_head)
    {
      struct delayed_set_stat *data = delayed_set_stat_head;
      bool skip_this_one = 0;
      struct stat st;
      mode_t current_mode = data->current_mode;
      mode_t current_mode_mask = data->current_mode_mask;

      check_for_renamed_directories |= data->after_links;

      if (after_links < data->after_links
	  || (data->file_name_len < file_name_len
	      && file_name[data->file_name_len]
	      && (ISSLASH (file_name[data->file_name_len])
		  || ISSLASH (file_name[data->file_name_len - 1]))
	      && memcmp (file_name, data->file_name, data->file_name_len) == 0))
	break;

      chdir_do (data->change_dir);

      if (check_for_renamed_directories)
	{
	  if (fstatat (chdir_fd, data->file_name, &st, data->atflag) != 0)
	    {
	      stat_error (data->file_name);
	      skip_this_one = 1;
	    }
	  else
	    {
	      current_mode = st.st_mode;
	      current_mode_mask = ALL_MODE_BITS;
	      if (! (st.st_dev == data->dev && st.st_ino == data->ino))
		{
		  ERROR ((0, 0,
			  _("%s: Directory renamed before its status could be extracted"),
			  quotearg_colon (data->file_name)));
		  skip_this_one = 1;
		}
	    }
	}

      if (! skip_this_one)
	{
	  struct tar_stat_info sb;
	  sb.stat.st_mode = data->mode;
	  sb.stat.st_uid = data->uid;
	  sb.stat.st_gid = data->gid;
	  sb.atime = data->atime;
	  sb.mtime = data->mtime;
	  sb.cntx_name = data->cntx_name;
	  sb.acls_a_ptr = data->acls_a_ptr;
	  sb.acls_a_len = data->acls_a_len;
	  sb.acls_d_ptr = data->acls_d_ptr;
	  sb.acls_d_len = data->acls_d_len;
	  sb.xattr_map = data->xattr_map;
	  sb.xattr_map_size = data->xattr_map_size;
	  set_stat (data->file_name, &sb,
		    -1, current_mode, current_mode_mask,
		    DIRTYPE, data->interdir, data->atflag);
	}

      delayed_set_stat_head = data->next;
      free_delayed_set_stat (data);
    }
}
