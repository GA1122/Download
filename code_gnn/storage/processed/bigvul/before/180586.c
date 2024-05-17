  static void record_recent_object(struct object *obj,
				 struct strbuf *path,
				 const char *last,
// 				 const char *name,
  				 void *data)
  {
  	sha1_array_append(&recent_objects, obj->oid.hash);
 }