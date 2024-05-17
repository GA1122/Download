static void openssl_print_object_sn(const char *s)
{
	ASN1_OBJECT *obj = OBJ_txt2obj(s, 0);
	if (obj) {
		int nid = OBJ_obj2nid(obj);
		if (nid != NID_undef) {
			printf(", %s", OBJ_nid2sn(nid));
		}
		ASN1_OBJECT_free(obj);
	}
}
