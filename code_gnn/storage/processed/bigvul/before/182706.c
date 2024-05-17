 xfs_setattr_nonsize(
 	struct xfs_inode	*ip,
 	struct iattr		*iattr,
 	int			flags)
 {
 	xfs_mount_t		*mp = ip->i_mount;
 	struct inode		*inode = VFS_I(ip);
 	int			mask = iattr->ia_valid;
 	xfs_trans_t		*tp;
 	int			error;
 	kuid_t			uid = GLOBAL_ROOT_UID, iuid = GLOBAL_ROOT_UID;
 	kgid_t			gid = GLOBAL_ROOT_GID, igid = GLOBAL_ROOT_GID;
 	struct xfs_dquot	*udqp = NULL, *gdqp = NULL;
 	struct xfs_dquot	*olddquot1 = NULL, *olddquot2 = NULL;
 
 	ASSERT((mask & ATTR_SIZE) == 0);
 
 	 
 	if (XFS_IS_QUOTA_ON(mp) && (mask & (ATTR_UID|ATTR_GID))) {
 		uint	qflags = 0;
 
 		if ((mask & ATTR_UID) && XFS_IS_UQUOTA_ON(mp)) {
 			uid = iattr->ia_uid;
 			qflags |= XFS_QMOPT_UQUOTA;
 		} else {
 			uid = inode->i_uid;
 		}
 		if ((mask & ATTR_GID) && XFS_IS_GQUOTA_ON(mp)) {
 			gid = iattr->ia_gid;
 			qflags |= XFS_QMOPT_GQUOTA;
 		}  else {
 			gid = inode->i_gid;
 		}
 
 		 
 		ASSERT(udqp == NULL);
 		ASSERT(gdqp == NULL);
 		error = xfs_qm_vop_dqalloc(ip, xfs_kuid_to_uid(uid),
 					   xfs_kgid_to_gid(gid),
 					   xfs_get_projid(ip),
 					   qflags, &udqp, &gdqp, NULL);
 		if (error)
 			return error;
 	}
 
 	error = xfs_trans_alloc(mp, &M_RES(mp)->tr_ichange, 0, 0, 0, &tp);
 	if (error)
 		goto out_dqrele;
 
 	xfs_ilock(ip, XFS_ILOCK_EXCL);
 	xfs_trans_ijoin(tp, ip, 0);
 
 	 
 	if (mask & (ATTR_UID|ATTR_GID)) {
 		 
 		iuid = inode->i_uid;
 		igid = inode->i_gid;
 		gid = (mask & ATTR_GID) ? iattr->ia_gid : igid;
 		uid = (mask & ATTR_UID) ? iattr->ia_uid : iuid;
 
 		 
 		if (XFS_IS_QUOTA_RUNNING(mp) &&
 		    ((XFS_IS_UQUOTA_ON(mp) && !uid_eq(iuid, uid)) ||
 		     (XFS_IS_GQUOTA_ON(mp) && !gid_eq(igid, gid)))) {
 			ASSERT(tp);
 			error = xfs_qm_vop_chown_reserve(tp, ip, udqp, gdqp,
 						NULL, capable(CAP_FOWNER) ?
 						XFS_QMOPT_FORCE_RES : 0);
 			if (error)	 
 				goto out_cancel;
 		}
 	}
 
 	 
 	if (mask & (ATTR_UID|ATTR_GID)) {
 		 
 		if ((inode->i_mode & (S_ISUID|S_ISGID)) &&
 		    !capable(CAP_FSETID))
 			inode->i_mode &= ~(S_ISUID|S_ISGID);
 
 		 
 		if (!uid_eq(iuid, uid)) {
 			if (XFS_IS_QUOTA_RUNNING(mp) && XFS_IS_UQUOTA_ON(mp)) {
 				ASSERT(mask & ATTR_UID);
 				ASSERT(udqp);
 				olddquot1 = xfs_qm_vop_chown(tp, ip,
 							&ip->i_udquot, udqp);
 			}
 			ip->i_d.di_uid = xfs_kuid_to_uid(uid);
 			inode->i_uid = uid;
 		}
 		if (!gid_eq(igid, gid)) {
 			if (XFS_IS_QUOTA_RUNNING(mp) && XFS_IS_GQUOTA_ON(mp)) {
 				ASSERT(xfs_sb_version_has_pquotino(&mp->m_sb) ||
 				       !XFS_IS_PQUOTA_ON(mp));
 				ASSERT(mask & ATTR_GID);
 				ASSERT(gdqp);
 				olddquot2 = xfs_qm_vop_chown(tp, ip,
 							&ip->i_gdquot, gdqp);
 			}
 			ip->i_d.di_gid = xfs_kgid_to_gid(gid);
 			inode->i_gid = gid;
 		}
 	}
 
 	if (mask & ATTR_MODE)
 		xfs_setattr_mode(ip, iattr);
 	if (mask & (ATTR_ATIME|ATTR_CTIME|ATTR_MTIME))
 		xfs_setattr_time(ip, iattr);
 
 	xfs_trans_log_inode(tp, ip, XFS_ILOG_CORE);
 
 	XFS_STATS_INC(mp, xs_ig_attrchg);
 
 	if (mp->m_flags & XFS_MOUNT_WSYNC)
 		xfs_trans_set_sync(tp);
 	error = xfs_trans_commit(tp);
 
 	xfs_iunlock(ip, XFS_ILOCK_EXCL);
 
 	 
 	xfs_qm_dqrele(olddquot1);
 	xfs_qm_dqrele(olddquot2);
 	xfs_qm_dqrele(udqp);
 	xfs_qm_dqrele(gdqp);
 
 	if (error)
 		return error;
 
 	 
 	if ((mask & ATTR_MODE) && !(flags & XFS_ATTR_NOACL)) {
 		error = posix_acl_chmod(inode, inode->i_mode);
 		if (error)
 			return error;
 	}
 
 	return 0;
  
  out_cancel:
  	xfs_trans_cancel(tp);
// 	xfs_iunlock(ip, XFS_ILOCK_EXCL);
  out_dqrele:
  	xfs_qm_dqrele(udqp);
  	xfs_qm_dqrele(gdqp);
 	return error;
 }