void InitFromDBCallback(GDataFileError expected_error,
                        GDataFileError actual_error) {
  EXPECT_EQ(expected_error, actual_error);
}
