CronTab::validateParameter( int attribute_idx, const char *parameter,
							MyString &error ) {
	bool ret = true;
	MyString temp(parameter);
	if ( CronTab::regex.match( temp ) ) {
		error  = "Invalid parameter value '";
		error += parameter;
		error += "' for ";
		error += CronTab::attributes[attribute_idx];
		ret = false;
	}
	return ( ret );
}
