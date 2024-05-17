const char *OBJ_nid2sn(int n)
	{
	ADDED_OBJ ad,*adp;
	ASN1_OBJECT ob;

	if ((n >= 0) && (n < NUM_NID))
		{
		if ((n != NID_undef) && (nid_objs[n].nid == NID_undef))
			{
			OBJerr(OBJ_F_OBJ_NID2SN,OBJ_R_UNKNOWN_NID);
			return(NULL);
			}
		return(nid_objs[n].sn);
		}
	else if (added == NULL)
		return(NULL);
	else
		{
		ad.type=ADDED_NID;
		ad.obj= &ob;
		ob.nid=n;
		adp=lh_ADDED_OBJ_retrieve(added,&ad);
		if (adp != NULL)
			return(adp->obj->sn);
		else
			{
			OBJerr(OBJ_F_OBJ_NID2SN,OBJ_R_UNKNOWN_NID);
			return(NULL);
			}
		}
	}