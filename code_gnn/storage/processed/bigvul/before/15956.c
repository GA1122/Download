ImportArrayTIFF ( const TIFF_Manager::TagInfo & tagInfo, const bool nativeEndian,
				  SXMPMeta * xmp, const char * xmpNS, const char * xmpProp )
{


	switch ( tagInfo.type ) {

		case kTIFF_ShortType :
			ImportArrayTIFF_Short ( tagInfo, nativeEndian, xmp, xmpNS, xmpProp );
			break;

		case kTIFF_LongType :
			ImportArrayTIFF_Long ( tagInfo, nativeEndian, xmp, xmpNS, xmpProp );
			break;

		case kTIFF_RationalType :
			ImportArrayTIFF_Rational ( tagInfo, nativeEndian, xmp, xmpNS, xmpProp );
			break;

		case kTIFF_SRationalType :
			ImportArrayTIFF_SRational ( tagInfo, nativeEndian, xmp, xmpNS, xmpProp );
			break;

		case kTIFF_ASCIIType :
			ImportArrayTIFF_ASCII ( tagInfo, xmp, xmpNS, xmpProp );
			break;

		case kTIFF_ByteType :
			ImportArrayTIFF_Byte ( tagInfo, xmp, xmpNS, xmpProp );
			break;

		case kTIFF_SByteType :
			ImportArrayTIFF_SByte ( tagInfo, xmp, xmpNS, xmpProp );
			break;

		case kTIFF_SShortType :
			ImportArrayTIFF_SShort ( tagInfo, nativeEndian, xmp, xmpNS, xmpProp );
			break;

		case kTIFF_SLongType :
			ImportArrayTIFF_SLong ( tagInfo, nativeEndian, xmp, xmpNS, xmpProp );
			break;

		case kTIFF_FloatType :
			ImportArrayTIFF_Float ( tagInfo, nativeEndian, xmp, xmpNS, xmpProp );
			break;

		case kTIFF_DoubleType :
			ImportArrayTIFF_Double ( tagInfo, nativeEndian, xmp, xmpNS, xmpProp );
			break;

	}

}	 
