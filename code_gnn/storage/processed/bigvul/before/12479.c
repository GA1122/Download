static unsigned long added_obj_hash(const ADDED_OBJ *ca)
	{
	const ASN1_OBJECT *a;
	int i;
	unsigned long ret=0;
	unsigned char *p;

	a=ca->obj;
	switch (ca->type)
		{
	case ADDED_DATA:
		ret=a->length<<20L;
		p=(unsigned char *)a->data;
		for (i=0; i<a->length; i++)
			ret^=p[i]<<((i*3)%24);
		break;
	case ADDED_SNAME:
		ret=lh_strhash(a->sn);
		break;
	case ADDED_LNAME:
		ret=lh_strhash(a->ln);
		break;
	case ADDED_NID:
		ret=a->nid;
		break;
	default:
		 
		return 0;
		}
	ret&=0x3fffffffL;
	ret|=((unsigned long)ca->type)<<30L;
	return(ret);
	}
