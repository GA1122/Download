static void append_buffer_info( MyString &url, const char *method, char const *path )
{
	MyString buffer_list;
	MyString buffer_string;
	MyString dir;
	MyString file;
	int s,bs,ps;
	int result;

	filename_split(path,dir,file);

	 
	if(!strncmp(path,"/dev/",5)) return;

	 
	pseudo_get_buffer_info( &s, &bs, &ps );

	 
	 

	if(Shadow->getJobAd()->LookupString(ATTR_BUFFER_FILES,buffer_list)) {
		if( filename_remap_find(buffer_list.Value(),path,buffer_string) ||
		    filename_remap_find(buffer_list.Value(),file.Value(),buffer_string) ) {

			 
			url += "buffer:";

			 
			result = sscanf(buffer_string.Value(),"(%d,%d)",&s,&bs);
			if( result==2 ) url += buffer_string;

			return;
		}
	}

	 
	 

	if( s>0 && bs>0 && strcmp(method,"local") && strcmp(method,"special")  ) {
		url += "buffer:";
	}
}
