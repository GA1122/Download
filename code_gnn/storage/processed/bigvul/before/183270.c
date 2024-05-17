 static void skel(const char *homedir, uid_t u, gid_t g) {
 	char *fname;
 	if (arg_zsh) {
 		if (asprintf(&fname, "%s/.zshrc", homedir) == -1)
 			errExit("asprintf");
 		struct stat s;
  		if (stat(fname, &s) == 0)
  			return;
// 		if (is_link(fname)) {  
// 			fprintf(stderr, "Error: invalid %s file\n", fname);
// 			exit(1);
// 		}
  		if (stat("/etc/skel/.zshrc", &s) == 0) {
			if (is_link("/etc/skel/.zshrc")) {
				fprintf(stderr, "Error: invalid /etc/skel/.zshrc file\n");
				exit(1);
			}
			if (copy_file("/etc/skel/.zshrc", fname) == 0) {
				if (chown(fname, u, g) == -1)
					errExit("chown");
				fs_logger("clone /etc/skel/.zshrc");
			}
// 			copy_file_as_user("/etc/skel/.zshrc", fname, u, g, 0644);  
// 			fs_logger("clone /etc/skel/.zshrc");
  		}
		else {  
			FILE *fp = fopen(fname, "w");
			if (fp) {
				fprintf(fp, "\n");
				fclose(fp);
				if (chown(fname, u, g) == -1)
					errExit("chown");
				if (chmod(fname, S_IRUSR | S_IWUSR) < 0)
					errExit("chown");
				fs_logger2("touch", fname);
			}
// 		else {
// 			touch_file_as_user(fname, u, g, 0644);
// 			fs_logger2("touch", fname);
  		}
  		free(fname);
  	}
 	else if (arg_csh) {
 		if (asprintf(&fname, "%s/.cshrc", homedir) == -1)
 			errExit("asprintf");
 		struct stat s;
  		if (stat(fname, &s) == 0)
  			return;
// 		if (is_link(fname)) {  
// 			fprintf(stderr, "Error: invalid %s file\n", fname);
// 			exit(1);
// 		}
  		if (stat("/etc/skel/.cshrc", &s) == 0) {
			if (is_link("/etc/skel/.cshrc")) {
				fprintf(stderr, "Error: invalid /etc/skel/.cshrc file\n");
				exit(1);
			}
			if (copy_file("/etc/skel/.cshrc", fname) == 0) {
				if (chown(fname, u, g) == -1)
					errExit("chown");
				fs_logger("clone /etc/skel/.cshrc");
			}
// 			copy_file_as_user("/etc/skel/.cshrc", fname, u, g, 0644);  
// 			fs_logger("clone /etc/skel/.cshrc");
  		}
		else {  
			 
			FILE *fp = fopen(fname, "w");
			if (fp) {
				fprintf(fp, "\n");
				fclose(fp);
				if (chown(fname, u, g) == -1)
					errExit("chown");
				if (chmod(fname, S_IRUSR | S_IWUSR) < 0)
					errExit("chown");
				fs_logger2("touch", fname);
			}
// 		else {
// 			touch_file_as_user(fname, u, g, 0644);
// 			fs_logger2("touch", fname);
  		}
  		free(fname);
  	}
 	else {
 		if (asprintf(&fname, "%s/.bashrc", homedir) == -1)
  			errExit("asprintf");
  		struct stat s;
		if (stat(fname, &s) == 0)
// 		if (stat(fname, &s) == 0) 
  			return;
// 		if (is_link(fname)) {  
// 			fprintf(stderr, "Error: invalid %s file\n", fname);
// 			exit(1);
// 		}
  		if (stat("/etc/skel/.bashrc", &s) == 0) {
			if (is_link("/etc/skel/.bashrc")) {
				fprintf(stderr, "Error: invalid /etc/skel/.bashrc file\n");
				exit(1);
			}
			if (copy_file("/etc/skel/.bashrc", fname) == 0) {
				 
				if (chown(fname, u, g) == -1)
					errExit("chown");
				fs_logger("clone /etc/skel/.bashrc");
			}
// 			copy_file_as_user("/etc/skel/.bashrc", fname, u, g, 0644);  
// 			fs_logger("clone /etc/skel/.bashrc");
  		}
  		free(fname);
  	}
 }