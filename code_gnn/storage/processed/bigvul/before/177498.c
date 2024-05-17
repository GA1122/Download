int SparseBitSet::CountLeadingZeros(element x) {
 return sizeof(element) <= sizeof(int) ? __builtin_clz(x) : __builtin_clzl(x);
}
