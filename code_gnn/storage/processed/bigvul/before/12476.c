int OBJ_txt2nid(const char *s)
{
	ASN1_OBJECT *obj;
	int nid;
	obj = OBJ_txt2obj(s, 0);
	nid = OBJ_obj2nid(obj);
	ASN1_OBJECT_free(obj);
	return nid;
}
