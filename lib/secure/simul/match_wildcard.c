/*
**  secure/simul/match_wildcard.c
**
**  Bubbs@Timewarp.
**  Iria@Dune : now accepts a string as second argument.
**  Bubbs@Dune : rewritten to use regular expressions instead of buggy sscanf().
*/

private nosave string* _wldcd_cache  = ({ 0, 0, });

public string* match_wildcard(string pattern, mixed input) {
  !stringp(pattern) && raise_error("match_wildcard(): Bad type to argument 1.\n");
  !(stringp(input) || pointerp(input)) && raise_error("match_wildcard(): Bad type to argument 2.\n");

  if (stringp(input))
    input = ({ input, });

  if (pattern != _wldcd_cache[0])
    _wldcd_cache =
      ({
        _wldcd_cache[0] = pattern,
        implode(map(regexplode(pattern, "\\?|\\*|\\.|\\+|\\[[^\\]]*\\]|\\[|\\]|\\(|\\)|\\^|\\$|\\||\\\\"),
          ([
            "?" : ".",
            "*" : ".*",
            "." : "\\.",
            "+" : "\\+",
            "[" : "\\[",
            "]" : "\\]",
            "(" : "\\(",
            ")" : "\\)",
            "^" : "\\^",
            "$" : "\\$",
            "|" : "\\|",
            "\\": "\\\\",
          ])), ""),
      });

  return regexp(input, "^" +_wldcd_cache[1] +"$");
}
