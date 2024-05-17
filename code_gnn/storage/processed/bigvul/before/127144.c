  void SetCard(const CreditCard& card) {
    std::vector<CreditCard> cards;
    cards.push_back(card);
    SetCards(&cards);
  }
