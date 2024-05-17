int MyProxyGetDelegationReaper(Service *, int exitPid, int exitStatus)
{
  dprintf (D_ALWAYS, "MyProxyGetDelegationReaper pid = %d, rc = %d\n", exitPid, exitStatus);

  credentials.Rewind(); 
  CredentialWrapper * cred_wrapper;
  X509CredentialWrapper * matched_entry = NULL;
  while (credentials.Next (cred_wrapper)) {
    if (cred_wrapper->cred->GetType() == X509_CREDENTIAL_TYPE) {
      if (((X509CredentialWrapper*)cred_wrapper)->get_delegation_pid == exitPid) {
	matched_entry = (X509CredentialWrapper*)cred_wrapper;
	break;
      }
    }
  }  

	if (matched_entry) {
		while (exitStatus != 0) {

		int read_fd = matched_entry->get_delegation_err_fd;	 
		off_t offset = lseek(read_fd, 0, SEEK_SET);	 
		if (offset == (off_t)-1) {
			dprintf (D_ALWAYS, "myproxy-get-delegation for proxy (%s, %s), "
					"stderr tmp file %s lseek() failed: %s\n", 
					matched_entry->cred->GetOwner(),
					matched_entry->cred->GetName(),
					matched_entry->get_delegation_err_filename,
					strerror(errno)
				);
			break;
		}

		struct stat statbuf;
		int status = fstat(read_fd, &statbuf);
		if (status == -1) {
			dprintf (D_ALWAYS, "myproxy-get-delegation for proxy (%s, %s), "
					"stderr tmp file %s fstat() failed: %s\n", 
					matched_entry->cred->GetOwner(),
					matched_entry->cred->GetName(),
					matched_entry->get_delegation_err_filename,
					strerror(errno)
				);
			break;
		}
		matched_entry->get_delegation_err_buff =
		(char *) calloc ( statbuf.st_size + 1, sizeof(char) );
		int bytes_read =
			read (
					read_fd,
					matched_entry->get_delegation_err_buff,
					statbuf.st_size
				);
		if (bytes_read < 0 ) {
			dprintf (D_ALWAYS, "myproxy-get-delegation for proxy (%s, %s), "
					"stderr tmp file %s read() failed: %s\n", 
					matched_entry->cred->GetOwner(),
					matched_entry->cred->GetName(),
					matched_entry->get_delegation_err_filename,
					strerror(errno)
				);
			break;
		}

		if ( WIFEXITED(exitStatus) ) {
			dprintf (D_ALWAYS, "myproxy-get-delegation for proxy (%s, %s),  "
				"exited with status %d, output (top):\n%s\n\n",
				matched_entry->cred->GetOwner(),
				matched_entry->cred->GetName(),
				WEXITSTATUS(exitStatus),
				matched_entry->get_delegation_err_buff
			);
			break;
		} else if ( WIFSIGNALED(exitStatus) ) {
			dprintf (D_ALWAYS, "myproxy-get-delegation for proxy (%s, %s),  "

				"terminated by signal %d, output (top):\n%s\n\n",
				matched_entry->cred->GetOwner(),
				matched_entry->cred->GetName(),
				WTERMSIG(exitStatus),
				matched_entry->get_delegation_err_buff
			);
			break;
		} else {
			dprintf (D_ALWAYS, "myproxy-get-delegation for proxy (%s, %s),  "

				"unknown status %d, output (top):\n%s\n\n",
				matched_entry->cred->GetOwner(),
				matched_entry->cred->GetName(),
				exitStatus,
				matched_entry->get_delegation_err_buff
			);
			break;
		}

    }  

	matched_entry->get_delegation_reset();
	} else {
		dprintf (D_ALWAYS,
			"Unable to find X509Credential entry for myproxy-get-delegation pid=%d\n", exitPid);
	}

   return TRUE;
}
