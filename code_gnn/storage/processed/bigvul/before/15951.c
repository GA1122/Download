ExportTIFF_StandardMappings ( XMP_Uns8 ifd, TIFF_Manager * tiff, const SXMPMeta & xmp )
{
	const bool nativeEndian = tiff->IsNativeEndian();
	TIFF_Manager::TagInfo tagInfo;
	std::string xmpValue;
	XMP_OptionBits xmpForm;

	const TIFF_MappingToXMP * mappings = 0;

	if ( ifd == kTIFF_PrimaryIFD ) {
		mappings = sPrimaryIFDMappings;
	} else if ( ifd == kTIFF_ExifIFD ) {
		mappings = sExifIFDMappings;
	} else if ( ifd == kTIFF_GPSInfoIFD ) {
		mappings = sGPSInfoIFDMappings;
	} else {
		XMP_Throw ( "Invalid IFD for standard mappings", kXMPErr_InternalFailure );
	}

	for ( size_t i = 0; mappings[i].id != 0xFFFF; ++i ) {

		try {	 

			const TIFF_MappingToXMP & mapInfo =  mappings[i];

			if ( mapInfo.exportMode == kExport_Never ) continue;
			if ( mapInfo.name[0] == 0 ) continue;	 

			bool haveTIFF = tiff->GetTag ( ifd, mapInfo.id, &tagInfo );
			if ( haveTIFF && (mapInfo.exportMode == kExport_InjectOnly) ) continue;
			
			bool haveXMP  = xmp.GetProperty ( mapInfo.ns, mapInfo.name, &xmpValue, &xmpForm );
			if ( ! haveXMP ) {
			
				if ( haveTIFF && (mapInfo.exportMode == kExport_Always) ) tiff->DeleteTag ( ifd, mapInfo.id );

			} else {
			
				XMP_Assert ( tagInfo.type != kTIFF_UndefinedType );	 
				if ( tagInfo.type == kTIFF_UndefinedType ) continue;
	
				const bool mapSingle = ((mapInfo.count == 1) || (mapInfo.type == kTIFF_ASCIIType));
				if ( mapSingle ) {
					if ( ! XMP_PropIsSimple ( xmpForm ) ) continue;	 
					ExportSingleTIFF ( tiff, ifd, mapInfo, nativeEndian, xmpValue );
				} else {
					if ( ! XMP_PropIsArray ( xmpForm ) ) continue;	 
					ExportArrayTIFF ( tiff, ifd, mapInfo, nativeEndian, xmp, mapInfo.ns, mapInfo.name );
				}
				
			}

		} catch ( ... ) {


		}

	}

}	 
