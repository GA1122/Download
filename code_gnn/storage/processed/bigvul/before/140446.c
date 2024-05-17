bool CheckDigestServerAuth(const AuthChallengeInfo* auth_challenge) {
  if (!auth_challenge)
    return false;
  EXPECT_FALSE(auth_challenge->is_proxy);
  EXPECT_EQ("www.google.com:80", auth_challenge->challenger.ToString());
  EXPECT_EQ("digestive", auth_challenge->realm);
  EXPECT_EQ("digest", auth_challenge->scheme);
  return true;
}