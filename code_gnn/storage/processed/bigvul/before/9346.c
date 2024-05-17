create_placeholder_file (char *file_name, bool is_symlink, bool *interdir_made)
{
  int fd;
  struct stat st;

  while ((fd = openat (chdir_fd, file_name, O_WRONLY | O_CREAT | O_EXCL, 0)) < 0)
    {
      switch (maybe_recoverable (file_name, false, interdir_made))
	{
	case RECOVER_OK:
	  continue;

	case RECOVER_SKIP:
	  return 0;

	case RECOVER_NO:
	  open_error (file_name);
	  return -1;
	}
      }

  if (fstat (fd, &st) != 0)
    {
      stat_error (file_name);
      close (fd);
    }
  else if (close (fd) != 0)
    close_error (file_name);
  else
    {
      struct delayed_set_stat *h;
      struct delayed_link *p =
	xmalloc (offsetof (struct delayed_link, target)
		 + strlen (current_stat_info.link_name)
		 + 1);
      p->next = delayed_link_head;
      delayed_link_head = p;
      p->dev = st.st_dev;
      p->ino = st.st_ino;
      p->birthtime = get_stat_birthtime (&st);
      p->is_symlink = is_symlink;
      if (is_symlink)
	{
	  p->mode = current_stat_info.stat.st_mode;
	  p->uid = current_stat_info.stat.st_uid;
	  p->gid = current_stat_info.stat.st_gid;
	  p->atime = current_stat_info.atime;
	  p->mtime = current_stat_info.mtime;
	}
      p->change_dir = chdir_current;
      p->sources = xmalloc (offsetof (struct string_list, string)
			    + strlen (file_name) + 1);
      p->sources->next = 0;
      strcpy (p->sources->string, file_name);
      p->cntx_name = NULL;
      assign_string (&p->cntx_name, current_stat_info.cntx_name);
      p->acls_a_ptr = NULL;
      p->acls_a_len = 0;
      p->acls_d_ptr = NULL;
      p->acls_d_len = 0;
      xheader_xattr_copy (&current_stat_info, &p->xattr_map, &p->xattr_map_size);
      strcpy (p->target, current_stat_info.link_name);

      h = delayed_set_stat_head;
      if (h && ! h->after_links
	  && strncmp (file_name, h->file_name, h->file_name_len) == 0
	  && ISSLASH (file_name[h->file_name_len])
	  && (last_component (file_name) == file_name + h->file_name_len + 1))
	mark_after_links (h);

      return 0;
    }

  return -1;
}
