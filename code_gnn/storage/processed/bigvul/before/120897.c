ChromeURLRequestContextGetter::ChromeURLRequestContextGetter(
    ChromeURLRequestContextFactory* factory)
    : factory_(factory) {
  DCHECK(factory);
}
