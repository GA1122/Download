static void complete_path( const char *short_path, MyString &full_path )
{
	if(short_path[0]==DIR_DELIM_CHAR) {
		full_path = short_path;
	} else {
		full_path.sprintf("%s%s%s",
						  Shadow->getIwd(),
						  DIR_DELIM_STRING,
						  short_path);
	}
}
