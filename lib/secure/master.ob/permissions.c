/*
 * Master Object - Permissions
 *
 */

int privilege_violation(string op, mixed who, mixed arg3, mixed arg4) {
    return 1;
}

int query_allow_shadow(object victim) {
    return 1;
}

int valid_exec(string name) {
    return 1;
}

int valid_query_snoop(object obj) {
    return 1;
}

int valid_snoop(object snoopee, object snooper) {
    return 1;
}

int valid_trace(string what, mixed arg) {
    return 1;
}