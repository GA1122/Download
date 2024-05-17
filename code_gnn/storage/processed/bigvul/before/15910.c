bool ASF_LegacyManager::CheckDigest ( const SXMPMeta& xmp )
{
	bool ret = false;

	if ( ! digestComputed ) this->ComputeDigest();

	std::string oldDigest;

	if ( xmp.GetProperty ( kXMP_NS_ASF, "NativeDigest", &oldDigest, 0 ) ) {
		ret = (digestStr == oldDigest);
	}

	return ret;

}
