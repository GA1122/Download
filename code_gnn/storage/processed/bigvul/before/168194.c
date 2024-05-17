void BubbleManager::RemoveBubbleManagerObserver(
    BubbleManagerObserver* observer) {
   observers_.RemoveObserver(observer);
 }
