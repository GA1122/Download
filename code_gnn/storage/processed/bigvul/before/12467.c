const void *OBJ_bsearch_ex_(const void *key, const void *base_, int num,
			    int size,
			    int (*cmp)(const void *, const void *),
			    int flags)
	{
	const char *base=base_;
	int l,h,i=0,c=0;
	const char *p = NULL;

	if (num == 0) return(NULL);
	l=0;
	h=num;
	while (l < h)
		{
		i=(l+h)/2;
		p= &(base[i*size]);
		c=(*cmp)(key,p);
		if (c < 0)
			h=i;
		else if (c > 0)
			l=i+1;
		else
			break;
		}
#ifdef CHARSET_EBCDIC
 
	if (c != 0)
		{
		for (i=0; i<num; ++i)
			{
			p= &(base[i*size]);
			c = (*cmp)(key,p);
			if (c == 0 || (c < 0 && (flags & OBJ_BSEARCH_VALUE_ON_NOMATCH)))
				return p;
			}
		}
#endif
	if (c != 0 && !(flags & OBJ_BSEARCH_VALUE_ON_NOMATCH))
		p = NULL;
	else if (c == 0 && (flags & OBJ_BSEARCH_FIRST_VALUE_ON_MATCH))
		{
		while(i > 0 && (*cmp)(key,&(base[(i-1)*size])) == 0)
			i--;
		p = &(base[i*size]);
		}
	return(p);
	}
