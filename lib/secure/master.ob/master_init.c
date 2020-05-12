/*
 * Master Object - Initialization
 *
 */

void inaugurate_master(int arg) {
    // DRIVER HOOKS
    return;
}

/*
 * Return the string to be used as root-uid. 
 * Under !strict_euids, the function is expendable.
 */
string get_master_uid() {
    return "1";
}

/*
 * Evaluate an argument given as option '-f' to the driver on startup 
 * from the command line. If several '-f' options are given, this 
 * function will be called sequentially with all given arguments.
 */
void flag(string arg) {
    debug_message(sprintf("master::flag(string arg) not implemented. Received '%s'.\n", arg));
}

string* epilog(int eflag) {
    string init_file;
    if(eflag) {
        debug_message("Skipping init file: "INIT_FILE"\n");
        return ({});
    }
    if((file_size(INIT_FILE) < 0) || (!init_file = read_file(INIT_FILE))) {
        debug_message("Unable to load init file: "INIT_FILE"\n");
        return ({});
    }
    debug_message("Loading init file: "INIT_FILE"\n");
    return explode(init_file, "\n");
}

/*
 * Load a file passed in from the epilog function.
 * 
 */ 
void preload(string filename) {
    int start_time;
    int finish_time;
  
    if(strlen(filename) > 0 && filename[0] != '#') {
        debug_message(sprintf("Preloading: %s ", filename));
        start_time = time();
        if(catch(load_object(filename))) {
            debug_message(" FAILED\n");
        } else {
            finish_time = time();
            debug_message(sprintf(" %.2fs\n",(current_time-last_time)));
        }
    }
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
