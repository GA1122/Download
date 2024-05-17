static int added_obj_cmp(const ADDED_OBJ *ca, const ADDED_OBJ *cb)
	{
	ASN1_OBJECT *a,*b;
	int i;

	i=ca->type-cb->type;
	if (i) return(i);
	a=ca->obj;
	b=cb->obj;
	switch (ca->type)
		{
	case ADDED_DATA:
		i=(a->length - b->length);
		if (i) return(i);
		return(memcmp(a->data,b->data,(size_t)a->length));
	case ADDED_SNAME:
		if (a->sn == NULL) return(-1);
		else if (b->sn == NULL) return(1);
		else return(strcmp(a->sn,b->sn));
	case ADDED_LNAME:
		if (a->ln == NULL) return(-1);
		else if (b->ln == NULL) return(1);
		else return(strcmp(a->ln,b->ln));
	case ADDED_NID:
		return(a->nid-b->nid);
	default:
		 
		return 0;
		}
	}
