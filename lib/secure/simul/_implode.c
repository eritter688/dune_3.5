/*
**  secure/simul/_implode.c
**
**  Nicer implode.
**  Bubbs@Dune.
**  Ported to DUNE_3.5 unchanged. - Math
*/

varargs string _implode(mixed *arr,string del,string last) {
    if(!last) return implode(arr,del);
    if(!arr || !pointerp(arr))      raise_error("Bad argument 1 to _implode()\n");
    if(!stringp(del))               raise_error("Bad argument 2 to _implode()\n");
    switch(sizeof(arr)) {
        case 0:     return "";
        case 1:     return to_string(arr[0]);
        default:    return sprintf("%s%s%s",implode(arr[0..<2],del),(!last || !stringp(last))?del:last,arr[<1]);
    }
    return (string)0;
}
