void server_options(char **args, int *argc_p)
{
	static char argstr[64];
	int ac = *argc_p;
	uchar where;
	char *arg;
	int i, x;

	 
	args[ac++] = "--server";

	if (daemon_over_rsh > 0) {
		args[ac++] = "--daemon";
		*argc_p = ac;
		 
		return;
	}

	if (!am_sender)
		args[ac++] = "--sender";

	x = 1;
	argstr[0] = '-';

	if (protect_args)
		argstr[x++] = 's';

	for (i = 0; i < verbose; i++)
		argstr[x++] = 'v';

	 
	if (make_backups)
		argstr[x++] = 'b';
	if (update_only)
		argstr[x++] = 'u';
	if (!do_xfers)  
		argstr[x++] = 'n';
	if (preserve_links)
		argstr[x++] = 'l';
	if ((xfer_dirs >= 2 && xfer_dirs < 4)
	 || (xfer_dirs && !recurse && (list_only || (delete_mode && am_sender))))
		argstr[x++] = 'd';
	if (am_sender) {
		if (keep_dirlinks)
			argstr[x++] = 'K';
		if (prune_empty_dirs)
			argstr[x++] = 'm';
		if (omit_dir_times)
			argstr[x++] = 'O';
		if (omit_link_times)
			argstr[x++] = 'J';
		if (fuzzy_basis) {
			argstr[x++] = 'y';
			if (fuzzy_basis > 1)
				argstr[x++] = 'y';
		}
	} else {
		if (copy_links)
			argstr[x++] = 'L';
		if (copy_dirlinks)
			argstr[x++] = 'k';
	}

	if (whole_file > 0)
		argstr[x++] = 'W';
	 

	if (preserve_hard_links) {
		argstr[x++] = 'H';
		if (preserve_hard_links > 1)
			argstr[x++] = 'H';
	}
	if (preserve_uid)
		argstr[x++] = 'o';
	if (preserve_gid)
		argstr[x++] = 'g';
	if (preserve_devices)  
		argstr[x++] = 'D';
	if (preserve_times)
		argstr[x++] = 't';
	if (preserve_perms)
		argstr[x++] = 'p';
	else if (preserve_executability && am_sender)
		argstr[x++] = 'E';
#ifdef SUPPORT_ACLS
	if (preserve_acls)
		argstr[x++] = 'A';
#endif
#ifdef SUPPORT_XATTRS
	if (preserve_xattrs) {
		argstr[x++] = 'X';
		if (preserve_xattrs > 1)
			argstr[x++] = 'X';
	}
#endif
	if (recurse)
		argstr[x++] = 'r';
	if (always_checksum)
		argstr[x++] = 'c';
	if (cvs_exclude)
		argstr[x++] = 'C';
	if (ignore_times)
		argstr[x++] = 'I';
	if (relative_paths)
		argstr[x++] = 'R';
	if (one_file_system) {
		argstr[x++] = 'x';
		if (one_file_system > 1)
			argstr[x++] = 'x';
	}
	if (sparse_files)
		argstr[x++] = 'S';
	if (do_compression == 1)
		argstr[x++] = 'z';

	set_allow_inc_recurse();

	 
	if (protocol_version >= 30) {
		 
#define eFlags argstr
		 
		eFlags[x++] = 'e';
#if SUBPROTOCOL_VERSION != 0
		if (protocol_version == PROTOCOL_VERSION) {
			x += snprintf(argstr+x, sizeof argstr - x,
				      "%d.%d",
				      PROTOCOL_VERSION, SUBPROTOCOL_VERSION);
		} else
#endif
			eFlags[x++] = '.';
		if (allow_inc_recurse)
			eFlags[x++] = 'i';
#ifdef CAN_SET_SYMLINK_TIMES
		eFlags[x++] = 'L';  
#endif
#ifdef ICONV_OPTION
		eFlags[x++] = 's';  
#endif
		eFlags[x++] = 'f';  
		eFlags[x++] = 'x';  
		eFlags[x++] = 'C';  
#undef eFlags
	}

	if (x >= (int)sizeof argstr) {  
		rprintf(FERROR, "argstr overflow in server_options().\n");
		exit_cleanup(RERR_MALLOC);
	}

	argstr[x] = '\0';

	if (x > 1)
		args[ac++] = argstr;

#ifdef ICONV_OPTION
	if (iconv_opt) {
		char *set = strchr(iconv_opt, ',');
		if (set)
			set++;
		else
			set = iconv_opt;
		if (asprintf(&arg, "--iconv=%s", set) < 0)
			goto oom;
		args[ac++] = arg;
	}
#endif

	if (protect_args && !local_server)  
		args[ac++] = NULL;

	if (list_only > 1)
		args[ac++] = "--list-only";

	 
	if (xfer_dirs && !recurse && delete_mode && am_sender)
		args[ac++] = "--no-r";

	if (do_compression && def_compress_level != Z_DEFAULT_COMPRESSION) {
		if (asprintf(&arg, "--compress-level=%d", def_compress_level) < 0)
			goto oom;
		args[ac++] = arg;
	}

	if (preserve_devices) {
		 
		if (!preserve_specials)
			args[ac++] = "--no-specials";  
	} else if (preserve_specials)
		args[ac++] = "--specials";

	 
	if (stdout_format && am_sender) {
		 
		if (stdout_format_has_i > 1)
			args[ac++] = "--log-format=%i%I";
		else if (stdout_format_has_i)
			args[ac++] = "--log-format=%i";
		else if (stdout_format_has_o_or_i)
			args[ac++] = "--log-format=%o";
		else if (!verbose)
			args[ac++] = "--log-format=X";
	}

	if (block_size) {
		if (asprintf(&arg, "-B%lu", block_size) < 0)
			goto oom;
		args[ac++] = arg;
	}

	if (io_timeout) {
		if (asprintf(&arg, "--timeout=%d", io_timeout) < 0)
			goto oom;
		args[ac++] = arg;
	}

	if (bwlimit) {
		if (asprintf(&arg, "--bwlimit=%d", bwlimit) < 0)
			goto oom;
		args[ac++] = arg;
	}

	if (backup_dir) {
		args[ac++] = "--backup-dir";
		args[ac++] = backup_dir;
	}

	 
	if (strcmp(backup_suffix, backup_dir ? "" : BACKUP_SUFFIX) != 0) {
		 
		if (asprintf(&arg, "--suffix=%s", backup_suffix) < 0)
			goto oom;
		args[ac++] = arg;
	}

	if (checksum_choice) {
		if (asprintf(&arg, "--checksum-choice=%s", checksum_choice) < 0)
			goto oom;
		args[ac++] = arg;
	}

	if (am_sender) {
		if (max_delete > 0) {
			if (asprintf(&arg, "--max-delete=%d", max_delete) < 0)
				goto oom;
			args[ac++] = arg;
		} else if (max_delete == 0)
			args[ac++] = "--max-delete=-1";
		if (min_size >= 0) {
			args[ac++] = "--min-size";
			args[ac++] = min_size_arg;
		}
		if (max_size >= 0) {
			args[ac++] = "--max-size";
			args[ac++] = max_size_arg;
		}
		if (delete_before)
			args[ac++] = "--delete-before";
		else if (delete_during == 2)
			args[ac++] = "--delete-delay";
		else if (delete_during)
			args[ac++] = "--delete-during";
		else if (delete_after)
			args[ac++] = "--delete-after";
		else if (delete_mode && !delete_excluded)
			args[ac++] = "--delete";
		if (delete_excluded)
			args[ac++] = "--delete-excluded";
		if (force_delete)
			args[ac++] = "--force";
		if (write_batch < 0)
			args[ac++] = "--only-write-batch=X";
		if (am_root > 1)
			args[ac++] = "--super";
		if (size_only)
			args[ac++] = "--size-only";
		if (do_stats)
			args[ac++] = "--stats";
	} else {
		if (skip_compress) {
			if (asprintf(&arg, "--skip-compress=%s", skip_compress) < 0)
				goto oom;
			args[ac++] = arg;
		}
	}

	 
	if (missing_args == 2)
		args[ac++] = "--delete-missing-args";
	else if (missing_args == 1 && !am_sender)
		args[ac++] = "--ignore-missing-args";

	if (modify_window_set && am_sender) {
		char *fmt = modify_window < 0 ? "-@%d" : "--modify-window=%d";
		if (asprintf(&arg, fmt, modify_window) < 0)
			goto oom;
		args[ac++] = arg;
	}

	if (checksum_seed) {
		if (asprintf(&arg, "--checksum-seed=%d", checksum_seed) < 0)
			goto oom;
		args[ac++] = arg;
	}

	if (partial_dir && am_sender) {
		if (partial_dir != tmp_partialdir) {
			args[ac++] = "--partial-dir";
			args[ac++] = partial_dir;
		}
		if (delay_updates)
			args[ac++] = "--delay-updates";
	} else if (keep_partial && am_sender)
		args[ac++] = "--partial";

	if (ignore_errors)
		args[ac++] = "--ignore-errors";

	if (copy_unsafe_links)
		args[ac++] = "--copy-unsafe-links";

	if (safe_symlinks)
		args[ac++] = "--safe-links";

	if (numeric_ids)
		args[ac++] = "--numeric-ids";

	if (use_qsort)
		args[ac++] = "--use-qsort";

	if (am_sender) {
		if (usermap) {
			if (asprintf(&arg, "--usermap=%s", usermap) < 0)
				goto oom;
			args[ac++] = arg;
		}

		if (groupmap) {
			if (asprintf(&arg, "--groupmap=%s", groupmap) < 0)
				goto oom;
			args[ac++] = arg;
		}

		if (ignore_existing)
			args[ac++] = "--ignore-existing";

		 
		if (ignore_non_existing)
			args[ac++] = "--existing";

		if (tmpdir) {
			args[ac++] = "--temp-dir";
			args[ac++] = tmpdir;
		}

		if (basis_dir[0]) {
			 
			for (i = 0; i < basis_dir_cnt; i++) {
				args[ac++] = dest_option;
				args[ac++] = basis_dir[i];
			}
		}
	}

	 
	where = (am_server ? W_CLI : W_SRV) | (am_sender ? W_REC : W_SND);
	arg = make_output_option(info_words, info_levels, where);
	if (arg)
		args[ac++] = arg;

	arg = make_output_option(debug_words, debug_levels, where);
	if (arg)
		args[ac++] = arg;

	if (append_mode) {
		if (append_mode > 1)
			args[ac++] = "--append";
		args[ac++] = "--append";
	} else if (inplace)
		args[ac++] = "--inplace";

	if (files_from && (!am_sender || filesfrom_host)) {
		if (filesfrom_host) {
			args[ac++] = "--files-from";
			args[ac++] = files_from;
			if (eol_nulls)
				args[ac++] = "--from0";
		} else {
			args[ac++] = "--files-from=-";
			args[ac++] = "--from0";
		}
		if (!relative_paths)
			args[ac++] = "--no-relative";
	}
	 
	if (relative_paths && !implied_dirs && (!am_sender || protocol_version >= 30))
		args[ac++] = "--no-implied-dirs";

	if (remove_source_files == 1)
		args[ac++] = "--remove-source-files";
	else if (remove_source_files)
		args[ac++] = "--remove-sent-files";

	if (preallocate_files && am_sender)
		args[ac++] = "--preallocate";

	if (ac > MAX_SERVER_ARGS) {  
		rprintf(FERROR, "argc overflow in server_options().\n");
		exit_cleanup(RERR_MALLOC);
	}

	if (do_compression > 1)
		args[ac++] = "--new-compress";

	if (remote_option_cnt) {
		int j;
		if (ac + remote_option_cnt > MAX_SERVER_ARGS) {
			rprintf(FERROR, "too many remote options specified.\n");
			exit_cleanup(RERR_SYNTAX);
		}
		for (j = 1; j <= remote_option_cnt; j++)
			args[ac++] = (char*)remote_options[j];
	}

	*argc_p = ac;
	return;

    oom:
	out_of_memory("server_options");
}