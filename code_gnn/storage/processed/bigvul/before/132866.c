  void AssertAllTilesRequired(PictureLayerTiling* tiling) {
    std::vector<Tile*> tiles = tiling->AllTilesForTesting();
    for (size_t i = 0; i < tiles.size(); ++i)
      EXPECT_TRUE(tiles[i]->required_for_activation()) << "i: " << i;
    EXPECT_GT(tiles.size(), 0u);
  }
