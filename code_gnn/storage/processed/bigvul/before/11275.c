_asn1_error_description_tag_error (asn1_node node, char *ErrorDescription)
{

  Estrcpy (ErrorDescription, ":: tag error near element '");
  _asn1_hierarchical_name (node, ErrorDescription + strlen (ErrorDescription),
			   ASN1_MAX_ERROR_DESCRIPTION_SIZE - 40);
  Estrcat (ErrorDescription, "'");

}
