/*
 * Master Object - Initialization
 *
 */
object simul_efun;


/*
 * This function is called in the master object after it has been
 * created and is fully functional.
 *
 * arg = 0 the mud just started, this is the first master of all.
 * arg = 1 the master was destructed and then reactivated (because a new one couldn’t be loaded).
 * arg = 2 the master was destructed and then reactivated, but additionally lost all variable contents.
 * arg = 3 this is a reloaded master.
 *
 * This function has to at least set up the driverhooks to use.
 * Also, any mudwho or wizlist handling has to be initialized here.
 */
void inaugurate_master(int arg) {

    set_driver_hook(H_MOVE_OBJECT0, "");

    set_driver_hook(H_LOAD_UIDS, "");
    set_driver_hook(H_CLONE_UIDS, "");

    set_driver_hook(H_CREATE_SUPER, OBJECT_CREATE_FUNCTION_NAME);
    set_driver_hook(H_CREATE_OB, OBJECT_CREATE_FUNCTION_NAME);
    set_driver_hook(H_CREATE_CLONE, OBJECT_CREATE_FUNCTION_NAME);
    set_driver_hook(H_RESET, OBJECT_RESET_FUNCTION_NAME);
    set_driver_hook(H_CLEAN_UP, OBJECT_CLEAN_FUNCTION_NAME);

    set_driver_hook(
            H_MODIFY_COMMAND,
            ([
                    "n":"north", "e":"east", "s":"south", "w":"west",
                    "ne":"northeast", "nw":"northwest", "se":"southeast", "sw":"southwest",
                    "u":"up", "d":"down"
            ])
    );
    set_driver_hook(H_MODIFY_COMMAND_FNAME, "modify_command");

    set_driver_hook(H_NOTIFY_FAIL, "What ?\n");

    set_driver_hook(
            H_INCLUDE_DIRS,
            ([
                    "/secure/include/",
                    "/include/"
            ])
    );

    set_driver_hook(H_AUTO_INCLUDE, "");

    set_driver_hook(H_ERQ_STOP, "");

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
 * Load the INIT_FILE into an array of strings, line for line.
 * Each line of the resulting array is then passed to the preload function.
 */
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
 * Empty strings and lines starting with # are not processed.
 */ 
void preload(string filename) {
    if((strlen(filename) > 0) && (filename[0] != '#')) {
        debug_message(sprintf("Preloading: %s =>", filename));
        if(catch(load_object(filename))) {
            debug_message(" FAILED!\n");
        } else {
            debug_message(" OK!\n");
        }
    }
}

/*
 * Load the simul_efun object(s) and return one or more paths of it.
 * Note that the object(s) must be loaded by this function!
 */
string get_simul_efun() {
    string fname;
    string error;

    if(simul_efun) {
        return efun::object_name(simul_efun);
    }

    fname = SIMUL_EFUN_FILE;
    if(error = catch(efun::load_object(fname))) {
        debug_message(sprintf("Couldn't load simul_efun object: %s\n", fname));
        debug_message(sprintf("%s\n", error[1..]));
        debug_message("Shutting down.\n");
        efun::shutdown();
        return "";
    }
    simul_efun=(object)fname;
    return fname;
}

/*
 * Currently not implemented.
 */

/*
 * Evaluate an argument given as option ‘-f’ to the driver.
 * If several ‘-f’ options are given, this function will be
 * called sequentially with all given arguments.
 */
void flag(string arg) {
    debug_message(sprintf("master::flag(string arg) not implemented. Received '%s'.\n", arg));
}

/*
 * Master was reloaded on external request by SIGUSR1.
 * It will be called after inaugurate_master(M) of course.
 * If you plan to do additional magic here, you’re welcome.
 */
void external_master_reload() {
    raise_error("master::external_master_reload() not implemented.\n");
}

/*
 * This function is called in an already destructed master object
 * if no new master object could be loaded. flag will be 1 if the
 * old master object could be reclaimed from the list of objects
 * that were marked for destruction but not yet terminated. If flag
 * is 0, all variables of the object have been set to 0 and must be re-initialized.
 *
 * After this function, inaugurate_master(M) will be applied again.
 */
void reactivate_destructed_master(int removed) {
    raise_error("master::reactivate_destructed_master(int removed) not implemented.\n");
}
