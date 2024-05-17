int ASF_LegacyManager::ExportLegacy ( const SXMPMeta& xmp )
{
	int changed = 0;
	objectsToExport = 0;
	legacyDiff = 0;

	std::string utf8;
	std::string utf16;
	XMP_OptionBits flags;

	if ( ! broadcastSet ) {
		if ( xmp.GetProperty ( kXMP_NS_XMP, "CreateDate", &utf8, &flags ) ) {
			std::string date;
			ConvertISODateToMSDate ( utf8, &date );
			if ( fields[fieldCreationDate] != date ) {
				legacyDiff += date.size();
				legacyDiff -= fields[fieldCreationDate].size();
				this->SetField ( fieldCreationDate, date );
				objectsToExport |= objectFileProperties;
				changed ++;
			}
		}
	}

	if ( xmp.GetLocalizedText ( kXMP_NS_DC, "title", "", "x-default", 0, &utf8, &flags ) ) {
		NormalizeStringTrailingNull ( utf8 );
		ToUTF16 ( (const UTF8Unit*)utf8.data(), utf8.size(), &utf16, false );
		if ( fields[fieldTitle] != utf16 ) {
			legacyDiff += utf16.size();
			legacyDiff -= fields[fieldTitle].size();
			this->SetField ( fieldTitle, utf16 );
			objectsToExport |= objectContentDescription;
			changed ++;
		}
	}

	utf8.clear();
	SXMPUtils::CatenateArrayItems ( xmp, kXMP_NS_DC, "creator", 0, 0, kXMPUtil_AllowCommas, &utf8 );
	if ( ! utf8.empty() ) {
		NormalizeStringTrailingNull ( utf8 );
		ToUTF16 ( (const UTF8Unit*)utf8.data(), utf8.size(), &utf16, false );
		if ( fields[fieldAuthor] != utf16 ) {
			legacyDiff += utf16.size();
			legacyDiff -= fields[fieldAuthor].size();
			this->SetField ( fieldAuthor, utf16 );
			objectsToExport |= objectContentDescription;
			changed ++;
		}
	}

	if ( xmp.GetLocalizedText ( kXMP_NS_DC, "rights", "", "x-default", 0, &utf8, &flags ) ) {
		NormalizeStringTrailingNull ( utf8 );
		ToUTF16 ( (const UTF8Unit*)utf8.data(), utf8.size(), &utf16, false );
		if ( fields[fieldCopyright] != utf16 ) {
			legacyDiff += utf16.size();
			legacyDiff -= fields[fieldCopyright].size();
			this->SetField ( fieldCopyright, utf16 );
			objectsToExport |= objectContentDescription;
			changed ++;
		}
	}

	if ( xmp.GetLocalizedText ( kXMP_NS_DC, "description", "", "x-default", 0, &utf8, &flags ) ) {
		NormalizeStringTrailingNull ( utf8 );
		ToUTF16 ( (const UTF8Unit*)utf8.data(), utf8.size(), &utf16, false );
		if ( fields[fieldDescription] != utf16 ) {
			legacyDiff += utf16.size();
			legacyDiff -= fields[fieldDescription].size();
			this->SetField ( fieldDescription, utf16 );
			objectsToExport |= objectContentDescription;
			changed ++;
		}
	}

	if ( xmp.GetProperty ( kXMP_NS_XMP_Rights, "WebStatement", &utf8, &flags ) ) {
		NormalizeStringTrailingNull ( utf8 );
		if ( fields[fieldCopyrightURL] != utf8 ) {
			legacyDiff += utf8.size();
			legacyDiff -= fields[fieldCopyrightURL].size();
			this->SetField ( fieldCopyrightURL, utf8 );
			objectsToExport |= objectContentBranding;
			changed ++;
		}
	}

#if ! Exclude_LicenseURL_Recon
	if ( xmp.GetProperty ( kXMP_NS_XMP_Rights, "Certificate", &utf8, &flags ) ) {
		NormalizeStringTrailingNull ( utf8 );
		if ( fields[fieldLicenseURL] != utf8 ) {
			legacyDiff += utf8.size();
			legacyDiff -= fields[fieldLicenseURL].size();
			this->SetField ( fieldLicenseURL, utf8 );
			objectsToExport |= objectContentEncryption;
			changed ++;
		}
	}
#endif

	int newObjects = (objectsToExport & !objectsExisting);

	if ( newObjects & objectContentDescription )
		legacyDiff += sizeContentDescription;
	if ( newObjects & objectContentBranding )
		legacyDiff += sizeContentBranding;
	if ( newObjects & objectContentEncryption )
		legacyDiff += sizeContentEncryption;

	ComputeDigest();

	return changed;

}
