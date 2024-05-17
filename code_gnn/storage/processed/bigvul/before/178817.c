 cifs_lookup(struct inode *parent_dir_inode, struct dentry *direntry,
 	    struct nameidata *nd)
 {
 	int xid;
 	int rc = 0;  
 	__u32 oplock = enable_oplocks ? REQ_OPLOCK : 0;
 	__u16 fileHandle = 0;
 	bool posix_open = false;
 	struct cifs_sb_info *cifs_sb;
 	struct tcon_link *tlink;
 	struct cifs_tcon *pTcon;
 	struct cifsFileInfo *cfile;
 	struct inode *newInode = NULL;
 	char *full_path = NULL;
 	struct file *filp;
 
 	xid = GetXid();
 
 	cFYI(1, "parent inode = 0x%p name is: %s and dentry = 0x%p",
 	      parent_dir_inode, direntry->d_name.name, direntry);
 
 	 
 
 	cifs_sb = CIFS_SB(parent_dir_inode->i_sb);
 	tlink = cifs_sb_tlink(cifs_sb);
 	if (IS_ERR(tlink)) {
 		FreeXid(xid);
 		return (struct dentry *)tlink;
 	}
 	pTcon = tlink_tcon(tlink);
 
 	 
 	if (!(cifs_sb->mnt_cifs_flags & CIFS_MOUNT_POSIX_PATHS)) {
 		int i;
 		for (i = 0; i < direntry->d_name.len; i++)
 			if (direntry->d_name.name[i] == '\\') {
 				cFYI(1, "Invalid file name");
 				rc = -EINVAL;
 				goto lookup_out;
 			}
 	}
 
 	 
 	if (nd && (nd->flags & LOOKUP_EXCL)) {
 		d_instantiate(direntry, NULL);
 		rc = 0;
 		goto lookup_out;
 	}
 
 	 
 	full_path = build_path_from_dentry(direntry);
 	if (full_path == NULL) {
 		rc = -ENOMEM;
 		goto lookup_out;
 	}
 
 	if (direntry->d_inode != NULL) {
 		cFYI(1, "non-NULL inode in lookup");
 	} else {
 		cFYI(1, "NULL inode in lookup");
 	}
 	cFYI(1, "Full path: %s inode = 0x%p", full_path, direntry->d_inode);
 
 	 
 	if (pTcon->unix_ext) {
 		if (nd && !(nd->flags & LOOKUP_DIRECTORY) &&
 		     (nd->flags & LOOKUP_OPEN) && !pTcon->broken_posix_open &&
 		     (nd->intent.open.file->f_flags & O_CREAT)) {
 			rc = cifs_posix_open(full_path, &newInode,
 					parent_dir_inode->i_sb,
 					nd->intent.open.create_mode,
 					nd->intent.open.file->f_flags, &oplock,
 					&fileHandle, xid);
 			 
			if ((rc == 0) || (rc == -ENOENT))
// 			switch (rc) {
// 			case 0:
// 				 
// 				if (newInode && !S_ISREG(newInode->i_mode)) {
// 					CIFSSMBClose(xid, pTcon, fileHandle);
// 					break;
// 				}
// 			case -ENOENT:
  				posix_open = true;
			else if ((rc == -EINVAL) || (rc != -EOPNOTSUPP))
// 			case -EOPNOTSUPP:
// 				break;
// 			default:
  				pTcon->broken_posix_open = true;
// 			}
  		}
  		if (!posix_open)
  			rc = cifs_get_inode_info_unix(&newInode, full_path,
 						parent_dir_inode->i_sb, xid);
 	} else
 		rc = cifs_get_inode_info(&newInode, full_path, NULL,
 				parent_dir_inode->i_sb, xid, NULL);
 
 	if ((rc == 0) && (newInode != NULL)) {
 		d_add(direntry, newInode);
 		if (posix_open) {
 			filp = lookup_instantiate_filp(nd, direntry,
 						       generic_file_open);
 			if (IS_ERR(filp)) {
 				rc = PTR_ERR(filp);
 				CIFSSMBClose(xid, pTcon, fileHandle);
 				goto lookup_out;
 			}
 
 			cfile = cifs_new_fileinfo(fileHandle, filp, tlink,
 						  oplock);
 			if (cfile == NULL) {
 				fput(filp);
 				CIFSSMBClose(xid, pTcon, fileHandle);
 				rc = -ENOMEM;
 				goto lookup_out;
 			}
 		}
 		 
 		renew_parental_timestamps(direntry);
 
 	} else if (rc == -ENOENT) {
 		rc = 0;
 		direntry->d_time = jiffies;
 		d_add(direntry, NULL);
 	 
 	} else if (rc != -EACCES) {
 		cERROR(1, "Unexpected lookup error %d", rc);
 		 
 	}
 
 lookup_out:
 	kfree(full_path);
 	cifs_put_tlink(tlink);
 	FreeXid(xid);
 	return ERR_PTR(rc);
 }