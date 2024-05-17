bool CheckNTLMServerAuth(const AuthChallengeInfo* auth_challenge) {
  if (!auth_challenge)
    return false;
  EXPECT_FALSE(auth_challenge->is_proxy);
  EXPECT_EQ("https://server", auth_challenge->challenger.Serialize());
  EXPECT_EQ(std::string(), auth_challenge->realm);
  EXPECT_EQ(kNtlmAuthScheme, auth_challenge->scheme);
  return true;
}
