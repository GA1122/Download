  void CreateDefaultSearchContextAndRequestSearchTerm() {
    base::string16 surrounding = base::UTF8ToUTF16("Barack Obama just spoke.");
    CreateSearchContextAndRequestSearchTerm("Barack Obama", surrounding, 0, 6);
  }
