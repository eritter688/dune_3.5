/*
 * Master Object - Initialization
 *
 */

void inaugurate_master(int arg) {
    // DRIVER HOOKS
    return;
}

/*
 * Return the string to be used as root-uid. Under !strict_euids, the function is expendable.
 */
string get_master_uid() {
    return "1";
}

/*
 * Evaluate an argument given as option '-f' to the driver on startup from the command line. If
 * several '-f' options are given, this function will be called sequentially with all given arguments.
 */
void flag(string arg) {
    debug_message(sprintf("master::flag(string arg) not implemented. Received '%s'.\n", arg));
}

string *epilog(int eflag) {
    return;
}

void preload(string file) {
    return;
}

void external_master_reload() {
    raise_error("master::external_master_reload() not implemented.\n");
}

void reactivate_destructed_master(int removed) {
    raise_error(
            sprintf("master::reactivate_destructed_master(int removed) not implemented. Received '%d'.\n", removed));
}

string get_simul_efun() {
    return;
}