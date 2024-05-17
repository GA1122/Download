handle_t *ext3_journal_start_sb(struct super_block *sb, int nblocks)
{
	journal_t *journal;

	if (sb->s_flags & MS_RDONLY)
		return ERR_PTR(-EROFS);

	 
	journal = EXT3_SB(sb)->s_journal;
	if (is_journal_aborted(journal)) {
		ext3_abort(sb, __func__,
			   "Detected aborted journal");
		return ERR_PTR(-EROFS);
	}

	return journal_start(journal, nblocks);
}
