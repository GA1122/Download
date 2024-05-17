ImportSingleTIFF ( const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
				   SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{


	switch ( tagInfo.type ) {

		case kTIFF_ShortType :
			ImportSingleTIFF_Short ( tagInfo, nativeEndian, xmp, xmpNS, xmpProp );
			break;

		case kTIFF_LongType :
			ImportSingleTIFF_Long ( tagInfo, nativeEndian, xmp, xmpNS, xmpProp );
			break;

		case kTIFF_RationalType :
			ImportSingleTIFF_Rational ( tagInfo, nativeEndian, xmp, xmpNS, xmpProp );
			break;

		case kTIFF_SRationalType :
			ImportSingleTIFF_SRational ( tagInfo, nativeEndian, xmp, xmpNS, xmpProp );
			break;

		case kTIFF_ASCIIType :
			ImportSingleTIFF_ASCII ( tagInfo, xmp, xmpNS, xmpProp );
			break;

		case kTIFF_ByteType :
			ImportSingleTIFF_Byte ( tagInfo, xmp, xmpNS, xmpProp );
			break;

		case kTIFF_SByteType :
			ImportSingleTIFF_SByte ( tagInfo, xmp, xmpNS, xmpProp );
			break;

		case kTIFF_SShortType :
			ImportSingleTIFF_SShort ( tagInfo, nativeEndian, xmp, xmpNS, xmpProp );
			break;

		case kTIFF_SLongType :
			ImportSingleTIFF_SLong ( tagInfo, nativeEndian, xmp, xmpNS, xmpProp );
			break;

		case kTIFF_FloatType :
			ImportSingleTIFF_Float ( tagInfo, nativeEndian, xmp, xmpNS, xmpProp );
			break;

		case kTIFF_DoubleType :
			ImportSingleTIFF_Double ( tagInfo, nativeEndian, xmp, xmpNS, xmpProp );
			break;

	}

}	 
