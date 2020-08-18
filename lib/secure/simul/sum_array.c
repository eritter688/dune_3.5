/*
**  secure/simul/sum_array.c
**
**  Sums an array.
**  Bubbs@Dune, sum_arrays()
**  sum_array() reworked by Iria@Dune.
**  Ported to DUNE_3.5 unchanged. - Math
*/

mixed sum_array(mixed *arr) {
    mixed ret;
    if(!pointerp(arr))
        raise_error("Bad argument 1 to sum_array()\n");
    if(!sizeof(arr))
        return 0;
    ret = arr[0];
    map(arr[1..],(: $2+=$1 :),&ret);
    return ret;
}

mixed *sum_arrays(mixed *a,mixed *b) {
    int i, minsz;
    mixed *ret, *src, *dest;

    if(!pointerp(a))
      return pointerp(b) && deep_copy(b);
    if(!pointerp(b))
      return 0;
    if(sizeof(a)>sizeof(b)) {
      src=b; dest=a;
    }
    else {
      src=a; dest=b;
    }
    for(i=sizeof(src), ret=deep_copy(src); ~i; ret[--i] +=dest[i]);
    ret += dest[sizeof(src)..];
    return ret;
}
