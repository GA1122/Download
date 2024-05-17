static NTSTATUS check_spnego_blob_complete(struct smbd_server_connection *sconn,
					   uint16 smbpid, uint16 vuid,
					   DATA_BLOB *pblob)
{
	struct pending_auth_data *pad = NULL;
	ASN1_DATA *data;
	size_t needed_len = 0;

	pad = get_pending_auth_data(sconn, smbpid);

	 
	if (pblob->length == 0) {
		 
		DEBUG(2,("check_spnego_blob_complete: zero blob length !\n"));
		delete_partial_auth(sconn, pad);
		return NT_STATUS_OK;
	}

	 
	if (pad) {
		DATA_BLOB tmp_blob;
		size_t copy_len = MIN(65536, pblob->length);

		 

		if (pad->partial_data.length + copy_len <
				pad->partial_data.length ||
		    pad->partial_data.length + copy_len < copy_len) {

			DEBUG(2,("check_spnego_blob_complete: integer wrap "
				"pad->partial_data.length = %u, "
				"copy_len = %u\n",
				(unsigned int)pad->partial_data.length,
				(unsigned int)copy_len ));

			delete_partial_auth(sconn, pad);
			return NT_STATUS_INVALID_PARAMETER;
		}

		DEBUG(10,("check_spnego_blob_complete: "
			"pad->partial_data.length = %u, "
			"pad->needed_len = %u, "
			"copy_len = %u, "
			"pblob->length = %u,\n",
			(unsigned int)pad->partial_data.length,
			(unsigned int)pad->needed_len,
			(unsigned int)copy_len,
			(unsigned int)pblob->length ));

		tmp_blob = data_blob(NULL,
				pad->partial_data.length + copy_len);

		 
		memcpy(tmp_blob.data,
			pad->partial_data.data,
			pad->partial_data.length);
		memcpy(tmp_blob.data + pad->partial_data.length,
			pblob->data,
			copy_len);

		 
		data_blob_free(&pad->partial_data);
		pad->partial_data = tmp_blob;
		ZERO_STRUCT(tmp_blob);

		 
		if (pblob->length >= pad->needed_len) {
			 
			data_blob_free(pblob);
			*pblob = pad->partial_data;
			ZERO_STRUCT(pad->partial_data);
			delete_partial_auth(sconn, pad);
			return NT_STATUS_OK;
		}

		 
		pad->needed_len -= copy_len;
		return NT_STATUS_MORE_PROCESSING_REQUIRED;
	}

	if ((pblob->data[0] != ASN1_APPLICATION(0)) &&
	    (pblob->data[0] != ASN1_CONTEXT(1))) {
		 
		return NT_STATUS_OK;
	}

	 

	data = asn1_init(NULL);
	if (data == NULL) {
		return NT_STATUS_NO_MEMORY;
	}

	asn1_load(data, *pblob);
	asn1_start_tag(data, pblob->data[0]);
	if (data->has_error || data->nesting == NULL) {
		asn1_free(data);
		 
		return NT_STATUS_OK;
	}

	 

	if (data->nesting->taglen + data->nesting->start < data->nesting->taglen ||
	    data->nesting->taglen + data->nesting->start < data->nesting->start) {

		DEBUG(2,("check_spnego_blob_complete: integer wrap "
			"data.nesting->taglen = %u, "
			"data.nesting->start = %u\n",
			(unsigned int)data->nesting->taglen,
			(unsigned int)data->nesting->start ));

		asn1_free(data);
		return NT_STATUS_INVALID_PARAMETER;
	}

	 

	needed_len = data->nesting->taglen + data->nesting->start;
	asn1_free(data);

	DEBUG(10,("check_spnego_blob_complete: needed_len = %u, "
		"pblob->length = %u\n",
		(unsigned int)needed_len,
		(unsigned int)pblob->length ));

	if (needed_len <= pblob->length) {
		 
		return NT_STATUS_OK;
	}

	 
	if (needed_len > 65536) {
		DEBUG(2,("check_spnego_blob_complete: needed_len "
			"too large (%u)\n",
			(unsigned int)needed_len ));
		return NT_STATUS_INVALID_PARAMETER;
	}

	 
	if (!(pad = SMB_MALLOC_P(struct pending_auth_data))) {
		return NT_STATUS_NO_MEMORY;
	}
	pad->needed_len = needed_len - pblob->length;
	pad->partial_data = data_blob(pblob->data, pblob->length);
	if (pad->partial_data.data == NULL) {
		SAFE_FREE(pad);
		return NT_STATUS_NO_MEMORY;
	}
	pad->smbpid = smbpid;
	pad->vuid = vuid;
	DLIST_ADD(sconn->smb1.pd_list, pad);

	return NT_STATUS_MORE_PROCESSING_REQUIRED;
}
