void ASF_LegacyManager::ComputeDigest()
{
	MD5_CTX context;
	MD5_Digest digest;
	char buffer[40];

	MD5Init ( &context );
	digestStr.clear();
	digestStr.reserve ( 160 );

	for ( int type=0; type < fieldLast; ++type ) {

		if (fields[type].size ( ) > 0 ) {
			snprintf ( buffer, sizeof(buffer), "%d,", type );
			digestStr.append ( buffer );
			MD5Update ( &context, (XMP_Uns8*)fields[type].data(), fields[type].size() );
		}

	}

	if( digestStr.size() > 0 ) digestStr[digestStr.size()-1] = ';';

	MD5Final ( digest, &context );

	size_t in, out;
	for ( in = 0, out = 0; in < 16; in += 1, out += 2 ) {
		XMP_Uns8 byte = digest[in];
		buffer[out]   = ReconcileUtils::kHexDigits [ byte >> 4 ];
		buffer[out+1] = ReconcileUtils::kHexDigits [ byte & 0xF ];
	}
	buffer[32] = 0;

	digestStr.append ( buffer );

	digestComputed = true;

}
