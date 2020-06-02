/*
 * Constrain an integer or float value between lower and upper limits.
 */
mixed clip(mixed value, mixed lower, mixed upper) {

    if((intp(value) || floatp(value)) &&
       (intp(lower) || floatp(lower)) &&
       (intp(upper) || floatp(upper))) {
        return max(lower, min(value, upper));
    }

    raise_error("Bad argument types to clip()\n");
}
