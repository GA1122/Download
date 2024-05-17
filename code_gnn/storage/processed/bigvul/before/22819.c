static int nfs_update_inode(struct inode *inode, struct nfs_fattr *fattr)
{
	struct nfs_server *server;
	struct nfs_inode *nfsi = NFS_I(inode);
	loff_t cur_isize, new_isize;
	unsigned long invalid = 0;
	unsigned long now = jiffies;

	dfprintk(VFS, "NFS: %s(%s/%ld ct=%d info=0x%x)\n",
			__func__, inode->i_sb->s_id, inode->i_ino,
			atomic_read(&inode->i_count), fattr->valid);

	if (nfsi->fileid != fattr->fileid)
		goto out_fileid;

	 
	if ((inode->i_mode & S_IFMT) != (fattr->mode & S_IFMT))
		goto out_changed;

	server = NFS_SERVER(inode);
	 
	if (S_ISDIR(inode->i_mode) &&
			!nfs_fsid_equal(&server->fsid, &fattr->fsid) &&
			!test_bit(NFS_INO_MOUNTPOINT, &nfsi->flags))
		server->fsid = fattr->fsid;

	 
	nfsi->read_cache_jiffies = fattr->time_start;

	nfsi->cache_validity &= ~(NFS_INO_INVALID_ATTR | NFS_INO_INVALID_ATIME
			| NFS_INO_REVAL_PAGECACHE);

	 
	nfs_wcc_update_inode(inode, fattr);

	 
	if (!(fattr->valid & NFS_ATTR_FATTR_V4)) {
		 
		if (!timespec_equal(&inode->i_mtime, &fattr->mtime)) {
			dprintk("NFS: mtime change on server for file %s/%ld\n",
					inode->i_sb->s_id, inode->i_ino);
			invalid |= NFS_INO_INVALID_ATTR|NFS_INO_INVALID_DATA;
			if (S_ISDIR(inode->i_mode))
				nfs_force_lookup_revalidate(inode);
		}
		 
		if (!timespec_equal(&inode->i_ctime, &fattr->ctime))
			invalid |= NFS_INO_INVALID_ATTR|NFS_INO_INVALID_ACCESS|NFS_INO_INVALID_ACL;
	} else if (nfsi->change_attr != fattr->change_attr) {
		dprintk("NFS: change_attr change on server for file %s/%ld\n",
				inode->i_sb->s_id, inode->i_ino);
		invalid |= NFS_INO_INVALID_ATTR|NFS_INO_INVALID_DATA|NFS_INO_INVALID_ACCESS|NFS_INO_INVALID_ACL;
		if (S_ISDIR(inode->i_mode))
			nfs_force_lookup_revalidate(inode);
	}

	 
 	new_isize = nfs_size_to_loff_t(fattr->size);
	cur_isize = i_size_read(inode);
	if (new_isize != cur_isize) {
		 
		if (nfsi->npages == 0 || new_isize > cur_isize) {
			i_size_write(inode, new_isize);
			invalid |= NFS_INO_INVALID_ATTR|NFS_INO_INVALID_DATA;
		}
		dprintk("NFS: isize change on server for file %s/%ld\n",
				inode->i_sb->s_id, inode->i_ino);
	}


	memcpy(&inode->i_mtime, &fattr->mtime, sizeof(inode->i_mtime));
	memcpy(&inode->i_ctime, &fattr->ctime, sizeof(inode->i_ctime));
	memcpy(&inode->i_atime, &fattr->atime, sizeof(inode->i_atime));
	nfsi->change_attr = fattr->change_attr;

	if ((inode->i_mode & S_IALLUGO) != (fattr->mode & S_IALLUGO) ||
	    inode->i_uid != fattr->uid ||
	    inode->i_gid != fattr->gid)
		invalid |= NFS_INO_INVALID_ATTR|NFS_INO_INVALID_ACCESS|NFS_INO_INVALID_ACL;

	if (inode->i_nlink != fattr->nlink)
		invalid |= NFS_INO_INVALID_ATTR;

	inode->i_mode = fattr->mode;
	inode->i_nlink = fattr->nlink;
	inode->i_uid = fattr->uid;
	inode->i_gid = fattr->gid;

	if (fattr->valid & (NFS_ATTR_FATTR_V3 | NFS_ATTR_FATTR_V4)) {
		 
		inode->i_blocks = nfs_calc_block_size(fattr->du.nfs3.used);
 	} else {
 		inode->i_blocks = fattr->du.nfs2.blocks;
 	}

	 
	if (invalid & NFS_INO_INVALID_ATTR) {
		nfs_inc_stats(inode, NFSIOS_ATTRINVALIDATE);
		nfsi->attrtimeo = NFS_MINATTRTIMEO(inode);
		nfsi->attrtimeo_timestamp = now;
		nfsi->attr_gencount = nfs_inc_attr_generation_counter();
	} else {
		if (!time_in_range(now, nfsi->attrtimeo_timestamp, nfsi->attrtimeo_timestamp + nfsi->attrtimeo)) {
			if ((nfsi->attrtimeo <<= 1) > NFS_MAXATTRTIMEO(inode))
				nfsi->attrtimeo = NFS_MAXATTRTIMEO(inode);
			nfsi->attrtimeo_timestamp = now;
		}
	}
	invalid &= ~NFS_INO_INVALID_ATTR;
	 
	if (!(S_ISREG(inode->i_mode) || S_ISDIR(inode->i_mode)
				|| S_ISLNK(inode->i_mode)))
		invalid &= ~NFS_INO_INVALID_DATA;
	if (!nfs_have_delegation(inode, FMODE_READ) ||
			(nfsi->cache_validity & NFS_INO_REVAL_FORCED))
		nfsi->cache_validity |= invalid;
	nfsi->cache_validity &= ~NFS_INO_REVAL_FORCED;

	return 0;
 out_changed:
	 
	printk(KERN_DEBUG "%s: inode %ld mode changed, %07o to %07o\n",
			__func__, inode->i_ino, inode->i_mode, fattr->mode);
 out_err:
	 
	nfs_invalidate_inode(inode);
	return -ESTALE;

 out_fileid:
	printk(KERN_ERR "NFS: server %s error: fileid changed\n"
		"fsid %s: expected fileid 0x%Lx, got 0x%Lx\n",
		NFS_SERVER(inode)->nfs_client->cl_hostname, inode->i_sb->s_id,
		(long long)nfsi->fileid, (long long)fattr->fileid);
	goto out_err;
}
