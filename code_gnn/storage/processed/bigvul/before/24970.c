CIFSSMBQAllEAs(const int xid, struct cifs_tcon *tcon,
		const unsigned char *searchName, const unsigned char *ea_name,
		char *EAData, size_t buf_size,
		const struct nls_table *nls_codepage, int remap)
{
		 
	TRANSACTION2_QPI_REQ *pSMB = NULL;
	TRANSACTION2_QPI_RSP *pSMBr = NULL;
	int rc = 0;
	int bytes_returned;
	int list_len;
	struct fealist *ea_response_data;
	struct fea *temp_fea;
	char *temp_ptr;
	char *end_of_smb;
	__u16 params, byte_count, data_offset;
	unsigned int ea_name_len = ea_name ? strlen(ea_name) : 0;

	cFYI(1, "In Query All EAs path %s", searchName);
QAllEAsRetry:
	rc = smb_init(SMB_COM_TRANSACTION2, 15, tcon, (void **) &pSMB,
		      (void **) &pSMBr);
	if (rc)
		return rc;

	if (pSMB->hdr.Flags2 & SMBFLG2_UNICODE) {
		list_len =
		    cifsConvertToUCS((__le16 *) pSMB->FileName, searchName,
				     PATH_MAX, nls_codepage, remap);
		list_len++;	 
		list_len *= 2;
	} else {	 
		list_len = strnlen(searchName, PATH_MAX);
		list_len++;	 
		strncpy(pSMB->FileName, searchName, list_len);
	}

	params = 2   + 4   + list_len  ;
	pSMB->TotalDataCount = 0;
	pSMB->MaxParameterCount = cpu_to_le16(2);
	 
	pSMB->MaxDataCount = cpu_to_le16(CIFSMaxBufSize);
	pSMB->MaxSetupCount = 0;
	pSMB->Reserved = 0;
	pSMB->Flags = 0;
	pSMB->Timeout = 0;
	pSMB->Reserved2 = 0;
	pSMB->ParameterOffset = cpu_to_le16(offsetof(
	struct smb_com_transaction2_qpi_req, InformationLevel) - 4);
	pSMB->DataCount = 0;
	pSMB->DataOffset = 0;
	pSMB->SetupCount = 1;
	pSMB->Reserved3 = 0;
	pSMB->SubCommand = cpu_to_le16(TRANS2_QUERY_PATH_INFORMATION);
	byte_count = params + 1   ;
	pSMB->TotalParameterCount = cpu_to_le16(params);
	pSMB->ParameterCount = pSMB->TotalParameterCount;
	pSMB->InformationLevel = cpu_to_le16(SMB_INFO_QUERY_ALL_EAS);
	pSMB->Reserved4 = 0;
	inc_rfc1001_len(pSMB, byte_count);
	pSMB->ByteCount = cpu_to_le16(byte_count);

	rc = SendReceive(xid, tcon->ses, (struct smb_hdr *) pSMB,
			 (struct smb_hdr *) pSMBr, &bytes_returned, 0);
	if (rc) {
		cFYI(1, "Send error in QueryAllEAs = %d", rc);
		goto QAllEAsOut;
	}


	 
	 

	rc = validate_t2((struct smb_t2_rsp *)pSMBr);
	if (rc || get_bcc(&pSMBr->hdr) < 4) {
		rc = -EIO;	 
		goto QAllEAsOut;
	}

	 
	 
	 
	 
	 

	data_offset = le16_to_cpu(pSMBr->t2.DataOffset);
	ea_response_data = (struct fealist *)
				(((char *) &pSMBr->hdr.Protocol) + data_offset);

	list_len = le32_to_cpu(ea_response_data->list_len);
	cFYI(1, "ea length %d", list_len);
	if (list_len <= 8) {
		cFYI(1, "empty EA list returned from server");
		goto QAllEAsOut;
	}

	 
	end_of_smb = (char *)pByteArea(&pSMBr->hdr) + get_bcc(&pSMBr->hdr);
	if ((char *)ea_response_data + list_len > end_of_smb) {
		cFYI(1, "EA list appears to go beyond SMB");
		rc = -EIO;
		goto QAllEAsOut;
	}

	 
	list_len -= 4;
	temp_fea = ea_response_data->list;
	temp_ptr = (char *)temp_fea;
	while (list_len > 0) {
		unsigned int name_len;
		__u16 value_len;

		list_len -= 4;
		temp_ptr += 4;
		 
		if (list_len < 0) {
			cFYI(1, "EA entry goes beyond length of list");
			rc = -EIO;
			goto QAllEAsOut;
		}

		name_len = temp_fea->name_len;
		value_len = le16_to_cpu(temp_fea->value_len);
		list_len -= name_len + 1 + value_len;
		if (list_len < 0) {
			cFYI(1, "EA entry goes beyond length of list");
			rc = -EIO;
			goto QAllEAsOut;
		}

		if (ea_name) {
			if (ea_name_len == name_len &&
			    strncmp(ea_name, temp_ptr, name_len) == 0) {
				temp_ptr += name_len + 1;
				rc = value_len;
				if (buf_size == 0)
					goto QAllEAsOut;
				if ((size_t)value_len > buf_size) {
					rc = -ERANGE;
					goto QAllEAsOut;
				}
				memcpy(EAData, temp_ptr, value_len);
				goto QAllEAsOut;
			}
		} else {
			 
			rc += (5 + 1 + name_len);
			if (rc < (int) buf_size) {
				memcpy(EAData, "user.", 5);
				EAData += 5;
				memcpy(EAData, temp_ptr, name_len);
				EAData += name_len;
				 
				*EAData = 0;
				++EAData;
			} else if (buf_size == 0) {
				 
			} else {
				 
				rc = -ERANGE;
				break;
			}
		}
		temp_ptr += name_len + 1 + value_len;
		temp_fea = (struct fea *)temp_ptr;
	}

	 
	if (ea_name)
		rc = -ENODATA;

QAllEAsOut:
	cifs_buf_release(pSMB);
	if (rc == -EAGAIN)
		goto QAllEAsRetry;

	return (ssize_t)rc;
}
