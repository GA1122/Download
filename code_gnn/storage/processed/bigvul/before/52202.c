static inline int numeric_entity_is_allowed(unsigned uni_cp, int document_type)
{
	 
	switch (document_type) {
	case ENT_HTML_DOC_HTML401:
		 
		return uni_cp <= 0x10FFFF;
	case ENT_HTML_DOC_HTML5:
		 
		 
		return (uni_cp >= 0x20 && uni_cp <= 0x7E) ||
			(uni_cp >= 0x09 && uni_cp <= 0x0C && uni_cp != 0x0B) ||  
			(uni_cp >= 0xA0 && uni_cp <= 0x10FFFF &&
				((uni_cp & 0xFFFF) < 0xFFFE) &&  
				(uni_cp < 0xFDD0 || uni_cp > 0xFDEF));  
	case ENT_HTML_DOC_XHTML:
	case ENT_HTML_DOC_XML1:
		 
		return unicode_cp_is_allowed(uni_cp, document_type);
	default:
		return 1;
	}
}
