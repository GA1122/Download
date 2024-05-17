int XfrmController::fillNlAttrXfrmAlgoAuth(const XfrmAlgo& inAlgo, nlattr_algo_auth* algo) {
 if (inAlgo.name.empty()) {  
 return 0;
 }

 int len = NLA_HDRLEN + sizeof(xfrm_algo_auth);
    strncpy(algo->auth.alg_name, inAlgo.name.c_str(), sizeof(algo->auth.alg_name));
    algo->auth.alg_key_len = inAlgo.key.size() * 8;  

    algo->auth.alg_trunc_len = inAlgo.truncLenBits;

    memcpy(algo->key, &inAlgo.key[0], inAlgo.key.size());
    len += inAlgo.key.size();

    fillXfrmNlaHdr(&algo->hdr, XFRMA_ALG_AUTH_TRUNC, len);
 return len;
}
