PolymorphicAction<FireCallbackAction> FireCallback(
    base::RepeatingCallback<void()> callback) {
  return MakePolymorphicAction(FireCallbackAction(callback));
}
