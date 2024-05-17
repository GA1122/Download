asn1_expand_octet_string (asn1_node definitions, asn1_node * element,
			  const char *octetName, const char *objectName)
{
  char name[2 * ASN1_MAX_NAME_SIZE + 1], value[ASN1_MAX_NAME_SIZE];
  int retCode = ASN1_SUCCESS, result;
  int len, len2, len3;
  asn1_node p2, aux = NULL;
  asn1_node octetNode = NULL, objectNode = NULL;
  char errorDescription[ASN1_MAX_ERROR_DESCRIPTION_SIZE];

  if ((definitions == NULL) || (*element == NULL))
    return ASN1_ELEMENT_NOT_FOUND;

  octetNode = asn1_find_node (*element, octetName);
  if (octetNode == NULL)
    return ASN1_ELEMENT_NOT_FOUND;
  if (type_field (octetNode->type) != ASN1_ETYPE_OCTET_STRING)
    return ASN1_ELEMENT_NOT_FOUND;
  if (octetNode->value == NULL)
    return ASN1_VALUE_NOT_FOUND;

  objectNode = asn1_find_node (*element, objectName);
  if (objectNode == NULL)
    return ASN1_ELEMENT_NOT_FOUND;

  if (type_field (objectNode->type) != ASN1_ETYPE_OBJECT_ID)
    return ASN1_ELEMENT_NOT_FOUND;

  if (objectNode->value == NULL)
    return ASN1_VALUE_NOT_FOUND;


   
  p2 = definitions->down;
  while (p2)
    {
      if ((type_field (p2->type) == ASN1_ETYPE_OBJECT_ID) &&
	  (p2->type & CONST_ASSIGN))
	{
	  strcpy (name, definitions->name);
	  strcat (name, ".");
	  strcat (name, p2->name);

	  len = sizeof (value);
	  result = asn1_read_value (definitions, name, value, &len);

	  if ((result == ASN1_SUCCESS)
	      && (!_asn1_strcmp (objectNode->value, value)))
	    {

	      p2 = p2->right;	 
	      while ((p2) && (p2->type & CONST_ASSIGN))
		p2 = p2->right;

	      if (p2)
		{
		  strcpy (name, definitions->name);
		  strcat (name, ".");
		  strcat (name, p2->name);

		  result = asn1_create_element (definitions, name, &aux);
		  if (result == ASN1_SUCCESS)
		    {
		      _asn1_cpy_name (aux, octetNode);
		      len2 =
			asn1_get_length_der (octetNode->value,
					     octetNode->value_len, &len3);
		      if (len2 < 0)
			return ASN1_DER_ERROR;

		      result =
			asn1_der_decoding (&aux, octetNode->value + len3,
					   len2, errorDescription);
		      if (result == ASN1_SUCCESS)
			{

			  _asn1_set_right (aux, octetNode->right);
			  _asn1_set_right (octetNode, aux);

			  result = asn1_delete_structure (&octetNode);
			  if (result == ASN1_SUCCESS)
			    {
			      aux = NULL;
			      break;
			    }
			  else
			    {	 
			      asn1_delete_structure (&aux);
			      retCode = result;
			      break;
			    }
			}
		      else
			{	 
			  retCode = result;
			  break;
			}
		    }
		  else
		    {		 
		      retCode = result;
		      break;
		    }
		}
	      else
		{		 
		  retCode = ASN1_VALUE_NOT_VALID;
		  break;
		}
	    }
	}

      p2 = p2->right;

    }

  if (!p2)
    retCode = ASN1_VALUE_NOT_VALID;

  return retCode;
}
