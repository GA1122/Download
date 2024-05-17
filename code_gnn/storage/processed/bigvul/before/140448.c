bool CheckNTLMServerAuth(const AuthChallengeInfo* auth_challenge) {
  if (!auth_challenge)
    return false;
  EXPECT_FALSE(auth_challenge->is_proxy);
  EXPECT_EQ("172.22.68.17:80", auth_challenge->challenger.ToString());
  EXPECT_EQ(std::string(), auth_challenge->realm);
  EXPECT_EQ("ntlm", auth_challenge->scheme);
  return true;
}
