void ASF_LegacyManager::ImportLegacy ( SXMPMeta* xmp )
{
	std::string utf8;

	if ( ! broadcastSet ) {
		ConvertMSDateToISODate ( fields[fieldCreationDate], &utf8 );
		if ( ! utf8.empty() ) xmp->SetProperty ( kXMP_NS_XMP, "CreateDate", utf8.c_str(), kXMP_DeleteExisting );
	}

	FromUTF16 ( (UTF16Unit*)fields[fieldTitle].c_str(), (fields[fieldTitle].size() / 2), &utf8, false );
	if ( ! utf8.empty() ) xmp->SetLocalizedText ( kXMP_NS_DC, "title", "", "x-default", utf8.c_str(), kXMP_DeleteExisting );

	xmp->DeleteProperty ( kXMP_NS_DC, "creator" );
	FromUTF16 ( (UTF16Unit*)fields[fieldAuthor].c_str(), (fields[fieldAuthor].size() / 2), &utf8, false );
	if ( ! utf8.empty() ) SXMPUtils::SeparateArrayItems ( xmp, kXMP_NS_DC, "creator",
														  (kXMP_PropArrayIsOrdered | kXMPUtil_AllowCommas), utf8.c_str() );

	FromUTF16 ( (UTF16Unit*)fields[fieldCopyright].c_str(), (fields[fieldCopyright].size() / 2), &utf8, false );
	if ( ! utf8.empty() ) xmp->SetLocalizedText ( kXMP_NS_DC, "rights", "", "x-default", utf8.c_str(), kXMP_DeleteExisting );

	FromUTF16 ( (UTF16Unit*)fields[fieldDescription].c_str(), (fields[fieldDescription].size() / 2), &utf8, false );
	if ( ! utf8.empty() ) xmp->SetLocalizedText ( kXMP_NS_DC, "description", "", "x-default", utf8.c_str(), kXMP_DeleteExisting );

	if ( ! fields[fieldCopyrightURL].empty() ) xmp->SetProperty ( kXMP_NS_XMP_Rights, "WebStatement", fields[fieldCopyrightURL].c_str(), kXMP_DeleteExisting );

#if ! Exclude_LicenseURL_Recon
	if ( ! fields[fieldLicenseURL].empty() ) xmp->SetProperty ( kXMP_NS_XMP_Rights, "Certificate", fields[fieldLicenseURL].c_str(), kXMP_DeleteExisting );
#endif

	imported = true;

}
