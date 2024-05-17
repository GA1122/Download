set_last_default(struct magic_set *ms, struct magic_entry *me, uint32_t nme)
{
	uint32_t i;
	for (i = 0; i < nme; i++) {
		if (me[i].mp->cont_level == 0 &&
		    me[i].mp->type == FILE_DEFAULT) {
			while (++i < nme)
				if (me[i].mp->cont_level == 0)
					break;
			if (i != nme) {
				 
				ms->line = me[i].mp->lineno;
				file_magwarn(ms,
				    "level 0 \"default\" did not sort last");
			}
			return;					    
		}
	}
}