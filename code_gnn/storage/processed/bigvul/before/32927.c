static journal_t *ext3_get_journal(struct super_block *sb,
				   unsigned int journal_inum)
{
	struct inode *journal_inode;
	journal_t *journal;

	 

	journal_inode = ext3_iget(sb, journal_inum);
	if (IS_ERR(journal_inode)) {
		ext3_msg(sb, KERN_ERR, "error: no journal found");
		return NULL;
	}
	if (!journal_inode->i_nlink) {
		make_bad_inode(journal_inode);
		iput(journal_inode);
		ext3_msg(sb, KERN_ERR, "error: journal inode is deleted");
		return NULL;
	}

	jbd_debug(2, "Journal inode found at %p: %Ld bytes\n",
		  journal_inode, journal_inode->i_size);
	if (!S_ISREG(journal_inode->i_mode)) {
		ext3_msg(sb, KERN_ERR, "error: invalid journal inode");
		iput(journal_inode);
		return NULL;
	}

	journal = journal_init_inode(journal_inode);
	if (!journal) {
		ext3_msg(sb, KERN_ERR, "error: could not load journal inode");
		iput(journal_inode);
		return NULL;
	}
	journal->j_private = sb;
	ext3_init_journal_params(sb, journal);
	return journal;
}