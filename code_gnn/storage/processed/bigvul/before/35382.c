parse_rock_ridge_inode_internal(struct iso_directory_record *de,
				struct inode *inode, int flags)
{
	int symlink_len = 0;
	int cnt, sig;
	unsigned int reloc_block;
	struct inode *reloc;
	struct rock_ridge *rr;
	int rootflag;
	struct rock_state rs;
	int ret = 0;

	if (!ISOFS_SB(inode->i_sb)->s_rock)
		return 0;

	init_rock_state(&rs, inode);
	setup_rock_ridge(de, inode, &rs);
	if (flags & RR_REGARD_XA) {
		rs.chr += 14;
		rs.len -= 14;
		if (rs.len < 0)
			rs.len = 0;
	}

repeat:
	while (rs.len > 2) {  
		rr = (struct rock_ridge *)rs.chr;
		 
		if (rr->len < 3)
			goto out;	 
		sig = isonum_721(rs.chr);
		if (rock_check_overflow(&rs, sig))
			goto eio;
		rs.chr += rr->len;
		rs.len -= rr->len;
		 
		if (rs.len < 0)
			goto out;	 

		switch (sig) {
#ifndef CONFIG_ZISOFS		 
		case SIG('R', 'R'):
			if ((rr->u.RR.flags[0] &
			     (RR_PX | RR_TF | RR_SL | RR_CL)) == 0)
				goto out;
			break;
#endif
		case SIG('S', 'P'):
			if (check_sp(rr, inode))
				goto out;
			break;
		case SIG('C', 'E'):
			rs.cont_extent = isonum_733(rr->u.CE.extent);
			rs.cont_offset = isonum_733(rr->u.CE.offset);
			rs.cont_size = isonum_733(rr->u.CE.size);
			break;
		case SIG('E', 'R'):
			ISOFS_SB(inode->i_sb)->s_rock = 1;
			printk(KERN_DEBUG "ISO 9660 Extensions: ");
			{
				int p;
				for (p = 0; p < rr->u.ER.len_id; p++)
					printk("%c", rr->u.ER.data[p]);
			}
			printk("\n");
			break;
		case SIG('P', 'X'):
			inode->i_mode = isonum_733(rr->u.PX.mode);
			set_nlink(inode, isonum_733(rr->u.PX.n_links));
			i_uid_write(inode, isonum_733(rr->u.PX.uid));
			i_gid_write(inode, isonum_733(rr->u.PX.gid));
			break;
		case SIG('P', 'N'):
			{
				int high, low;
				high = isonum_733(rr->u.PN.dev_high);
				low = isonum_733(rr->u.PN.dev_low);
				 
				if ((low & ~0xff) && high == 0) {
					inode->i_rdev =
					    MKDEV(low >> 8, low & 0xff);
				} else {
					inode->i_rdev =
					    MKDEV(high, low);
				}
			}
			break;
		case SIG('T', 'F'):
			 
			 
			cnt = 0;
			if (rr->u.TF.flags & TF_CREATE) {
				inode->i_ctime.tv_sec =
				    iso_date(rr->u.TF.times[cnt++].time,
					     0);
				inode->i_ctime.tv_nsec = 0;
			}
			if (rr->u.TF.flags & TF_MODIFY) {
				inode->i_mtime.tv_sec =
				    iso_date(rr->u.TF.times[cnt++].time,
					     0);
				inode->i_mtime.tv_nsec = 0;
			}
			if (rr->u.TF.flags & TF_ACCESS) {
				inode->i_atime.tv_sec =
				    iso_date(rr->u.TF.times[cnt++].time,
					     0);
				inode->i_atime.tv_nsec = 0;
			}
			if (rr->u.TF.flags & TF_ATTRIBUTES) {
				inode->i_ctime.tv_sec =
				    iso_date(rr->u.TF.times[cnt++].time,
					     0);
				inode->i_ctime.tv_nsec = 0;
			}
			break;
		case SIG('S', 'L'):
			{
				int slen;
				struct SL_component *slp;
				struct SL_component *oldslp;
				slen = rr->len - 5;
				slp = &rr->u.SL.link;
				inode->i_size = symlink_len;
				while (slen > 1) {
					rootflag = 0;
					switch (slp->flags & ~1) {
					case 0:
						inode->i_size +=
						    slp->len;
						break;
					case 2:
						inode->i_size += 1;
						break;
					case 4:
						inode->i_size += 2;
						break;
					case 8:
						rootflag = 1;
						inode->i_size += 1;
						break;
					default:
						printk("Symlink component flag "
							"not implemented\n");
					}
					slen -= slp->len + 2;
					oldslp = slp;
					slp = (struct SL_component *)
						(((char *)slp) + slp->len + 2);

					if (slen < 2) {
						if (((rr->u.SL.
						      flags & 1) != 0)
						    &&
						    ((oldslp->
						      flags & 1) == 0))
							inode->i_size +=
							    1;
						break;
					}

					 
					if (!rootflag
					    && (oldslp->flags & 1) == 0)
						inode->i_size += 1;
				}
			}
			symlink_len = inode->i_size;
			break;
		case SIG('R', 'E'):
			printk(KERN_WARNING "Attempt to read inode for "
					"relocated directory\n");
			goto out;
		case SIG('C', 'L'):
			if (flags & RR_RELOC_DE) {
				printk(KERN_ERR
				       "ISOFS: Recursive directory relocation "
				       "is not supported\n");
				goto eio;
			}
			reloc_block = isonum_733(rr->u.CL.location);
			if (reloc_block == ISOFS_I(inode)->i_iget5_block &&
			    ISOFS_I(inode)->i_iget5_offset == 0) {
				printk(KERN_ERR
				       "ISOFS: Directory relocation points to "
				       "itself\n");
				goto eio;
			}
			ISOFS_I(inode)->i_first_extent = reloc_block;
			reloc = isofs_iget_reloc(inode->i_sb, reloc_block, 0);
			if (IS_ERR(reloc)) {
				ret = PTR_ERR(reloc);
				goto out;
			}
			inode->i_mode = reloc->i_mode;
			set_nlink(inode, reloc->i_nlink);
			inode->i_uid = reloc->i_uid;
			inode->i_gid = reloc->i_gid;
			inode->i_rdev = reloc->i_rdev;
			inode->i_size = reloc->i_size;
			inode->i_blocks = reloc->i_blocks;
			inode->i_atime = reloc->i_atime;
			inode->i_ctime = reloc->i_ctime;
			inode->i_mtime = reloc->i_mtime;
			iput(reloc);
			break;
#ifdef CONFIG_ZISOFS
		case SIG('Z', 'F'): {
			int algo;

			if (ISOFS_SB(inode->i_sb)->s_nocompress)
				break;
			algo = isonum_721(rr->u.ZF.algorithm);
			if (algo == SIG('p', 'z')) {
				int block_shift =
					isonum_711(&rr->u.ZF.parms[1]);
				if (block_shift > 17) {
					printk(KERN_WARNING "isofs: "
						"Can't handle ZF block "
						"size of 2^%d\n",
						block_shift);
				} else {
					 
					ISOFS_I(inode)->i_file_format =
						isofs_file_compressed;
					 
					ISOFS_I(inode)->i_format_parm[0] =
						isonum_711(&rr->u.ZF.parms[0]);
					ISOFS_I(inode)->i_format_parm[1] =
						isonum_711(&rr->u.ZF.parms[1]);
					inode->i_size =
					    isonum_733(rr->u.ZF.
						       real_size);
				}
			} else {
				printk(KERN_WARNING
				       "isofs: Unknown ZF compression "
						"algorithm: %c%c\n",
				       rr->u.ZF.algorithm[0],
				       rr->u.ZF.algorithm[1]);
			}
			break;
		}
#endif
		default:
			break;
		}
	}
	ret = rock_continue(&rs);
	if (ret == 0)
		goto repeat;
	if (ret == 1)
		ret = 0;
out:
	kfree(rs.buffer);
	return ret;
eio:
	ret = -EIO;
	goto out;
}