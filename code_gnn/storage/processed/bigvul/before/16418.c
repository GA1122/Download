CStarter::createJobOwnerSecSession( int  , Stream* s )
{

	MyString fqu;
	getJobOwnerFQUOrDummy(fqu);
	ASSERT( !fqu.IsEmpty() );

	MyString error_msg;
	ClassAd input;
	s->decode();
	if( !input.initFromStream(*s) || !s->end_of_message() ) {
		dprintf(D_ALWAYS,"Failed to read request in createJobOwnerSecSession()\n");
		return FALSE;
	}

	MyString job_claim_id;
	MyString input_claim_id;
	getJobClaimId(job_claim_id);
	input.LookupString(ATTR_CLAIM_ID,input_claim_id);
	if( job_claim_id != input_claim_id || job_claim_id.IsEmpty() ) {
		dprintf(D_ALWAYS,
				"Claim ID provided to createJobOwnerSecSession does not match "
				"expected value!  Rejecting connection from %s\n",
				s->peer_description());
		return FALSE;
	}

	char *session_id = Condor_Crypt_Base::randomHexKey();
	char *session_key = Condor_Crypt_Base::randomHexKey();

	MyString session_info;
	input.LookupString(ATTR_SESSION_INFO,session_info);


	IpVerify* ipv = daemonCore->getSecMan()->getIpVerify();
	bool rc = ipv->PunchHole(READ, fqu);
	if( !rc ) {
		error_msg = "Starter failed to create authorization entry for job owner.";
	}

	if( rc ) {
		rc = daemonCore->getSecMan()->CreateNonNegotiatedSecuritySession(
			READ,
			session_id,
			session_key,
			session_info.Value(),
			fqu.Value(),
			NULL,
			0 );
	}
	if( rc ) {
		session_info = "";
		rc = daemonCore->getSecMan()->ExportSecSessionInfo(
			session_id,
			session_info );
	}

	ClassAd response;
	response.Assign(ATTR_VERSION,CondorVersion());
	if( !rc ) {
		if( error_msg.IsEmpty() ) {
			error_msg = "Failed to create security session.";
		}
		response.Assign(ATTR_RESULT,false);
		response.Assign(ATTR_ERROR_STRING,error_msg);
		dprintf(D_ALWAYS,
				"createJobOwnerSecSession failed: %s\n", error_msg.Value());
	}
	else {

		ClaimIdParser claimid(session_id,session_info.Value(),session_key);
		response.Assign(ATTR_RESULT,true);
		response.Assign(ATTR_CLAIM_ID,claimid.claimId());
		response.Assign(ATTR_STARTER_IP_ADDR,daemonCore->publicNetworkIpAddr());

		dprintf(D_FULLDEBUG,"Created security session for job owner (%s).\n",
				fqu.Value());
	}

	if( !response.put(*s) || !s->end_of_message() ) {
		dprintf(D_ALWAYS,
				"createJobOwnerSecSession failed to send response\n");
	}

	free( session_id );
	free( session_key );

	return TRUE;
}
