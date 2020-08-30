/*
**  secure/simul/timezones.c
**
**  Bubbs@Dune.
**  Port to Dune 3.5 -- Math
*/
#define MUD_ZONE_OFFSET         -5.0       // Host offset from GMT

float query_zone_offset(string tzone) {
    float offset;
    if(!tzone)
        return 0.0;
    // these are all the negative of what they should be, I think.
    switch(lower_case(tzone)) {
        case "yst" :  offset =  -9.0;  break;  // -9h   Yukon Standard
        case "pst" :  offset =  -8.0;  break;  // -8h   Pacific Standard
        case "mst" :  offset =  -7.0;  break;  // -7h   Mountain Standard
        case "cst" :  offset =  -6.0;  break;  // -6h   Central Standard
        case "est" :  offset =  -5.0;  break;  // -5h   Eastern Standard
        case "ast" :  offset =  -4.0;  break;  // -4h   Atlantic Standard
        case "nst" :  offset =  -3.5;  break;  // -3.5h Newfoundland Standard
        case "wet" :                           //       Western European
        case "pwt" :                           //       Portugese Winter
        case "gmt" :                           //       Greenwich Mean
        case "utc" :  offset =   0.0;  break;  //       Universal Time Co-ordinate
        case "bst" :  offset =   1.0;  break;  // +1h   British Summer
        case "met" :                           // +1h   Middle European
        case "cet" :  offset =   1.0;  break;  // +1h   Central European
        case "cest":  offset =   2.0;  break;  // +2h   Central European Summer
        case "sast":                           //       South Africa Standard
        case "eet" :  offset =   2.0;  break;  // +2h   Eastern European
        case "ist" :  offset =   5.5;  break;  // +5.5h Indian Standard
        case "awt" :  offset =   8.0;  break;  // +8h   Australian Western
        case "act" :  offset =   9.5;  break;  // +9.5h Australian Central
        case "aet" :  offset =  10.0;  break;  // +10h  Australian Eastern
        case "nzst":                           // +12h  New Zealand Standard
        case "nzt" :  offset =  12.0;  break;  // +12h  New Zealand
        default    :
            offset  = to_float(tzone);
            offset -= to_float(12 * to_int(offset/12));
    }
    return offset - MUD_ZONE_OFFSET;
}

varargs int user_time(mixed arg,int base_time) {
    string zone;
    if(!arg)  arg=this_player();
    if(!arg)  return time();
    if(objectp(arg))        zone=(string)arg->query_env_var("timezone");
    else if(stringp(arg))   zone=arg;
    else                    raise_error("user_time() : Bad type to argument 1.\n");
    if(!intp(base_time))    raise_error("user_time() : Bad type to optional argument 2.\n");
    if(!base_time)          base_time=time();
    return (base_time+to_int(3600.0*query_zone_offset(zone)));
}

//varargs int localtime(mixed arg,int base_time) { return user_time(arg, base_time); }
