  virtual ~TestPrerenderContents() {
    EXPECT_EQ(expected_final_status_, final_status()) <<
        " when testing URL " << prerender_url().path();
     MessageLoopForUI::current()->Quit();
   }
