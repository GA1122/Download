CStarter::createTempExecuteDir( void )
{

	if( is_gridshell ) { 
		dprintf( D_ALWAYS, "gridshell running in: \"%s\"\n", WorkingDir.Value() ); 
		return true;
	}

#ifndef WIN32
	bool use_chown = false;
	if (can_switch_ids()) {
		struct stat st;
		if (stat(Execute, &st) == -1) {
			EXCEPT("stat failed on %s: %s",
			       Execute,
			       strerror(errno));
		}
		if (!(st.st_mode & S_IWOTH)) {
			use_chown = true;
		}
	}
	priv_state priv;
	if (use_chown) {
		priv = set_condor_priv();
	}
	else {
		priv = set_user_priv();
	}
#else
	priv_state priv = set_condor_priv();
#endif

	CondorPrivSepHelper* cpsh = condorPrivSepHelper();
	if (cpsh != NULL) {
		cpsh->initialize_sandbox(WorkingDir.Value());
		WriteAdFiles();
	} else {
		if( mkdir(WorkingDir.Value(), 0777) < 0 ) {
			dprintf( D_FAILURE|D_ALWAYS,
			         "couldn't create dir %s: %s\n",
			         WorkingDir.Value(),
			         strerror(errno) );
			set_priv( priv );
			return false;
		}
		WriteAdFiles();
#if !defined(WIN32)
		if (use_chown) {
			priv_state p = set_root_priv();
			if (chown(WorkingDir.Value(),
			          get_user_uid(),
			          get_user_gid()) == -1)
			{
				EXCEPT("chown error on %s: %s",
				       WorkingDir.Value(),
				       strerror(errno));
			}
			set_priv(p);
		}
#endif
	}

#ifdef WIN32
	{
		canonicalize_dir_delimiters(WorkingDir);

		perm dirperm;
		const char * nobody_login = get_user_loginname();
		ASSERT(nobody_login);
		dirperm.init(nobody_login);
		bool ret_val = dirperm.set_acls( WorkingDir.Value() );
		if ( !ret_val ) {
			dprintf(D_ALWAYS,"UNABLE TO SET PERMISSIONS ON EXECUTE DIRECTORY\n");
			set_priv( priv );
			return false;
		}
	}
	
	
	if ( param_boolean_crufty("ENCRYPT_EXECUTE_DIRECTORY", false) ) {
		
			
			typedef BOOL (WINAPI *FPEncryptionDisable)(LPCWSTR,BOOL);
			typedef BOOL (WINAPI *FPEncryptFileA)(LPCSTR);
			bool efs_support = true;
			
			HINSTANCE advapi = LoadLibrary("ADVAPI32.dll");
			if ( !advapi ) {
				dprintf(D_FULLDEBUG, "Can't load advapi32.dll\n");
				efs_support = false;
			}
			FPEncryptionDisable EncryptionDisable = (FPEncryptionDisable) 
				GetProcAddress(advapi,"EncryptionDisable");
			if ( !EncryptionDisable ) {
				dprintf(D_FULLDEBUG, "cannot get address for EncryptionDisable()");
				efs_support = false;
			}
			FPEncryptFileA EncryptFile = (FPEncryptFileA) 
				GetProcAddress(advapi,"EncryptFileA");
			if ( !EncryptFile ) {
				dprintf(D_FULLDEBUG, "cannot get address for EncryptFile()");
				efs_support = false;
			}

			if ( efs_support ) {
				wchar_t *WorkingDir_w = new wchar_t[WorkingDir.Length()+1];
				swprintf(WorkingDir_w, L"%S", WorkingDir.Value());
				EncryptionDisable(WorkingDir_w, FALSE);
				delete[] WorkingDir_w;
				
				if ( EncryptFile(WorkingDir.Value()) == 0 ) {
					dprintf(D_ALWAYS, "Could not encrypt execute directory "
							"(err=%li)\n", GetLastError());
				}

				FreeLibrary(advapi);  

			} else {
				dprintf(D_ALWAYS, "ENCRYPT_EXECUTE_DIRECTORY set to True, "
						"but the Encryption" " functions are unavailable!");
			}

	}  
	

#endif  

	if( chdir(WorkingDir.Value()) < 0 ) {
		dprintf( D_FAILURE|D_ALWAYS, "couldn't move to %s: %s\n", WorkingDir.Value(),
				 strerror(errno) ); 
		set_priv( priv );
		return false;
	}
	dprintf( D_FULLDEBUG, "Done moving to directory \"%s\"\n", WorkingDir.Value() );
	set_priv( priv );
	return true;
}
