static int attr_list_has_file( const char *attr, const char *path )
{
	char const *file;
	MyString str;

	file = condor_basename(path);

	Shadow->getJobAd()->LookupString(attr,str);
	StringList list(str.Value());

	if( list.contains_withwildcard(path) || list.contains_withwildcard(file) ) {
		return 1;
	} else {
		return 0;
	}
}
