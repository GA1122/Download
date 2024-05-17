find_global()
{
	MyString	file;
	file.sprintf( "%s_config", myDistro->Get() );
	return find_file( EnvGetName( ENV_CONFIG), file.Value() );
}
