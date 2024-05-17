  GURL GetTabURL() {
    GURL url;
    EXPECT_TRUE(tab_->GetCurrentURL(&url));
    return url;
   }
