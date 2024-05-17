bool TradQT_Manager::ImportSimpleXMP ( XMP_Uns32 id, SXMPMeta * xmp, XMP_StringPtr ns, XMP_StringPtr prop ) const
{

	try {

		InfoMapCPos infoPos = this->parsedBoxes.find ( id );
		if ( infoPos == this->parsedBoxes.end() ) return false;
		if ( infoPos->second.values.empty() ) return false;
		
		std::string xmpValue, tempValue;
		XMP_OptionBits flags;
		bool xmpExists = xmp->GetProperty ( ns, prop, &xmpValue, &flags );
		if ( xmpExists && (! XMP_PropIsSimple ( flags )) ) {
			XMP_Throw ( "TradQT_Manager::ImportSimpleXMP - XMP property must be simple", kXMPErr_BadParam );
		}
		
		bool convertOK;
		const ValueInfo & qtItem = infoPos->second.values[0];	 
	
		if ( xmpExists ) {
			convertOK = ConvertToMacLang ( xmpValue, qtItem.macLang, &tempValue );
			if ( ! convertOK ) return false;	 
			if ( tempValue == qtItem.macValue ) return false;	 
		}
	
		convertOK = ConvertFromMacLang ( qtItem.macValue, qtItem.macLang, &tempValue );
		if ( ! convertOK ) return false;	 
		xmp->SetProperty ( ns, prop, tempValue.c_str() );
		return true;
	
	} catch ( ... ) {

		return false;	 
	
	}
		
}	 
