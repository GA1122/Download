bool ImageLoader::getImageAnimationPolicy(ImageAnimationPolicy& policy) {
  if (!element()->document().settings())
    return false;

  policy = element()->document().settings()->getImageAnimationPolicy();
  return true;
}
