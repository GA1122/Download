bool CheckBasicProxyAuth(const AuthChallengeInfo* auth_challenge) {
  if (!auth_challenge)
    return false;
  EXPECT_TRUE(auth_challenge->is_proxy);
  EXPECT_EQ("http://myproxy:70", auth_challenge->challenger.Serialize());
  EXPECT_EQ("MyRealm1", auth_challenge->realm);
  EXPECT_EQ(kBasicAuthScheme, auth_challenge->scheme);
  return true;
}
