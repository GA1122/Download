user_path_parent(int dfd, const char __user *path,
		 struct path *parent,
		 struct qstr *last,
		 int *type,
		 unsigned int flags)
{
	 
	return filename_parentat(dfd, getname(path), flags & LOOKUP_REVAL,
				 parent, last, type);
}