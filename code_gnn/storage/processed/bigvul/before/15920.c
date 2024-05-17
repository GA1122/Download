unsigned int ASF_LegacyManager::GetFieldMaxSize ( fieldType field )
{
	unsigned int maxSize = 0;

	switch ( field ) {

		case fieldCreationDate :
			maxSize = 8;
			break;

		case fieldTitle :
		case fieldAuthor :
		case fieldCopyright :
		case fieldDescription :
			maxSize = 0xFFFF;
			break;

		case fieldCopyrightURL :
#if ! Exclude_LicenseURL_Recon
		case fieldLicenseURL :
#endif
			maxSize = 0xFFFFFFFF;
			break;

		default:
			break;

	}

	return maxSize;

}
