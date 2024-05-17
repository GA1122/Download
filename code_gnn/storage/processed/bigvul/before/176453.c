static bool checkIfCover(const GifImageDesc& target, const GifImageDesc& covered) {
 return target.Left <= covered.Left
 && covered.Left + covered.Width <= target.Left + target.Width
 && target.Top <= covered.Top
 && covered.Top + covered.Height <= target.Top + target.Height;
}
