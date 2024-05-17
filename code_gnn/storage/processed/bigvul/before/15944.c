ExportSingleTIFF ( TIFF_Manager * tiff, XMP_Uns8 ifd, const TIFF_MappingToXMP & mapInfo,
				   bool nativeEndian, const std::string & xmpValue )
{
	XMP_Assert ( (mapInfo.count == 1) || (mapInfo.type == kTIFF_ASCIIType) );
	XMP_Assert ( mapInfo.name[0] != 0 );	 
	
	bool ok;
	char nextChar;	 
	
	switch ( mapInfo.type ) {

		case kTIFF_ByteType : {
			unsigned short binValue;
			int items = sscanf ( xmpValue.c_str(), "%hu%c", &binValue, &nextChar );	 
			if ( items != 1 ) return;	 
			tiff->SetTag_Byte ( ifd, mapInfo.id, (XMP_Uns8)binValue );
			break;
		}

		case kTIFF_ShortType : {
			unsigned long binValue;
			int items = sscanf ( xmpValue.c_str(), "%lu%c", &binValue, &nextChar );	 
			if ( items != 1 ) return;	 
			tiff->SetTag_Short ( ifd, mapInfo.id, (XMP_Uns16)binValue );
			break;
		}

		case kTIFF_LongType : {
			unsigned long binValue;
			int items = sscanf ( xmpValue.c_str(), "%lu%c", &binValue, &nextChar );	 
			if ( items != 1 ) return;	 
			tiff->SetTag_Long ( ifd, mapInfo.id, (XMP_Uns32)binValue );
			break;
		}

		case kTIFF_ShortOrLongType : {
			unsigned long binValue;
			int items = sscanf ( xmpValue.c_str(), "%lu%c", &binValue, &nextChar );	 
			if ( items != 1 ) return;	 
			if ( binValue <= 0xFFFF ) {
				tiff->SetTag_Short ( ifd, mapInfo.id, (XMP_Uns16)binValue );
			} else {
				tiff->SetTag_Long ( ifd, mapInfo.id, (XMP_Uns32)binValue );
			}
			break;
		}

		case kTIFF_RationalType : {	 
			XMP_Uns32 num, denom;
			ok = DecodeRational ( xmpValue.c_str(), &num, &denom );
			if ( ! ok ) return;	 
			tiff->SetTag_Rational ( ifd, mapInfo.id, num, denom );
			break;
		}

		case kTIFF_SRationalType : {	 
			signed long num, denom;
			int items = sscanf ( xmpValue.c_str(), "%ld/%ld%c", &num, &denom, &nextChar );	 
			if ( items != 2 ) {
				if ( items != 1 ) return;	 
				denom = 1;	 
			}
			tiff->SetTag_SRational ( ifd, mapInfo.id, (XMP_Int32)num, (XMP_Int32)denom );
			break;
		}

		case kTIFF_ASCIIType :
			tiff->SetTag ( ifd, mapInfo.id, kTIFF_ASCIIType, (XMP_Uns32)(xmpValue.size()+1), xmpValue.c_str() );
			break;
			
		default:
			XMP_Assert ( false );	 

	}

}	 
