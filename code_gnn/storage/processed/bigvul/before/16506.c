bool param(MyString &buf,char const *param_name,char const *default_value)
{
	bool found = false;
	char *param_value = param(param_name);
	if( param_value ) {
		buf = param_value;
		found = true;
	}
	else if( default_value ) {
		buf = default_value;
	}
	free( param_value );
	return found;
}
