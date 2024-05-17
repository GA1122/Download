match(struct magic_set *ms, struct magic *magic, uint32_t nmagic,
    const unsigned char *s, size_t nbytes, size_t offset, int mode, int text,
    int flip, int recursion_level, int *printed_something, int *need_separator,
    int *returnval)
{
	uint32_t magindex = 0;
	unsigned int cont_level = 0;
	int returnvalv = 0, e;  
	int firstline = 1;  
	int print = (ms->flags & (MAGIC_MIME|MAGIC_APPLE)) == 0;

	if (returnval == NULL)
		returnval = &returnvalv;

	if (file_check_mem(ms, cont_level) == -1)
		return -1;

	for (magindex = 0; magindex < nmagic; magindex++) {
		int flush = 0;
		struct magic *m = &magic[magindex];

		if (m->type != FILE_NAME)
		if ((IS_STRING(m->type) &&
#define FLT (STRING_BINTEST | STRING_TEXTTEST)
		     ((text && (m->str_flags & FLT) == STRING_BINTEST) ||
		      (!text && (m->str_flags & FLT) == STRING_TEXTTEST))) ||
		    (m->flag & mode) != mode) {
			 
			while (magindex + 1 < nmagic &&
                               magic[magindex + 1].cont_level != 0 &&
			       ++magindex)
				continue;
			continue;  
		}

		ms->offset = m->offset;
		ms->line = m->lineno;

		 
		switch (mget(ms, s, m, nbytes, offset, cont_level, mode, text,
		    flip, recursion_level + 1, printed_something,
		    need_separator, returnval)) {
		case -1:
			return -1;
		case 0:
			flush = m->reln != '!';
			break;
		default:
			if (m->type == FILE_INDIRECT)
				*returnval = 1;

			switch (magiccheck(ms, m)) {
			case -1:
				return -1;
			case 0:
				flush++;
				break;
			default:
				flush = 0;
				break;
			}
			break;
		}
		if (flush) {
			 
			while (magindex < nmagic - 1 &&
			    magic[magindex + 1].cont_level != 0)
				magindex++;
			continue;
		}

		if ((e = handle_annotation(ms, m)) != 0) {
			*need_separator = 1;
			*printed_something = 1;
			*returnval = 1;
			return e;
		}
		 
		if (*m->desc) {
			*need_separator = 1;
			*printed_something = 1;
			if (print_sep(ms, firstline) == -1)
				return -1;
		}


		if (print && mprint(ms, m) == -1)
			return -1;

		ms->c.li[cont_level].off = moffset(ms, m);

		 
		if (file_check_mem(ms, ++cont_level) == -1)
			return -1;

		while (magindex + 1 < nmagic &&
		    magic[magindex + 1].cont_level != 0) {
			m = &magic[++magindex];
			ms->line = m->lineno;  

			if (cont_level < m->cont_level)
				continue;
			if (cont_level > m->cont_level) {
				 
				cont_level = m->cont_level;
			}
			ms->offset = m->offset;
			if (m->flag & OFFADD) {
				ms->offset +=
				    ms->c.li[cont_level - 1].off;
			}

#ifdef ENABLE_CONDITIONALS
			if (m->cond == COND_ELSE ||
			    m->cond == COND_ELIF) {
				if (ms->c.li[cont_level].last_match == 1)
					continue;
			}
#endif
			switch (mget(ms, s, m, nbytes, offset, cont_level, mode,
			    text, flip, recursion_level + 1, printed_something,
			    need_separator, returnval)) {
			case -1:
				return -1;
			case 0:
				if (m->reln != '!')
					continue;
				flush = 1;
				break;
			default:
				if (m->type == FILE_INDIRECT)
					*returnval = 1;
				flush = 0;
				break;
			}

			switch (flush ? 1 : magiccheck(ms, m)) {
			case -1:
				return -1;
			case 0:
#ifdef ENABLE_CONDITIONALS
				ms->c.li[cont_level].last_match = 0;
#endif
				break;
			default:
#ifdef ENABLE_CONDITIONALS
				ms->c.li[cont_level].last_match = 1;
#endif
				if (m->type == FILE_CLEAR)
					ms->c.li[cont_level].got_match = 0;
				else if (ms->c.li[cont_level].got_match) {
					if (m->type == FILE_DEFAULT)
						break;
				} else
					ms->c.li[cont_level].got_match = 1;
				if ((e = handle_annotation(ms, m)) != 0) {
					*need_separator = 1;
					*printed_something = 1;
					*returnval = 1;
					return e;
				}
				 
				if (*m->desc) {
					if (!*printed_something) {
						*printed_something = 1;
						if (print_sep(ms, firstline)
						    == -1)
							return -1;
					}
				}
				 
				 
				if (*need_separator
				    && ((m->flag & NOSPACE) == 0)
				    && *m->desc) {
					if (print &&
					    file_printf(ms, " ") == -1)
						return -1;
					*need_separator = 0;
				}
				if (print && mprint(ms, m) == -1)
					return -1;

				ms->c.li[cont_level].off = moffset(ms, m);

				if (*m->desc)
					*need_separator = 1;

				 
				if (file_check_mem(ms, ++cont_level) == -1)
					return -1;
				break;
			}
		}
		if (*printed_something) {
			firstline = 0;
			if (print)
				*returnval = 1;
		}
		if ((ms->flags & MAGIC_CONTINUE) == 0 && *printed_something) {
			return *returnval;  
		}
	}
	return *returnval;   
}
