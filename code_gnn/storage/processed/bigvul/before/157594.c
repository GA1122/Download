bool CheckBasicServerAuth(const AuthChallengeInfo* auth_challenge) {
  if (!auth_challenge)
    return false;
  EXPECT_FALSE(auth_challenge->is_proxy);
  EXPECT_EQ("http://www.example.org", auth_challenge->challenger.Serialize());
  EXPECT_EQ("MyRealm1", auth_challenge->realm);
  EXPECT_EQ(kBasicAuthScheme, auth_challenge->scheme);
  return true;
}