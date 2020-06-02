#define NAME_PROMPT     "Please enter your name: "
#define PASS_PROMPT     "Password: "



/*
 * Entry into the logon/creation process.
 */
void logon() {

    string *tmp, str;

    // Only valid entry point should be master::connect().
    if((previous_object() != this_object()) &&
       (previous_object() != find_object(__MASTER_OBJECT__))) {
        efun::destruct(this_object());
        return;
    }

    /*
     * Display Dune MUD Splash Screens
     */

    // TODO
    // Keep original ASCII art screens, add some Dune chapter preamble quotes.
    write("Welcome to Dune MUD (ldmud 3.6.2)\n");

    /*
     * Display MUD status: locked, etc.
     */

//    login_data=([]);
//    if(login_data[I_MSG]=LOGIND->query_banner())
//        printf("%s\n",login_data[I_MSG]);

    /*
     * Display short logged-in list.
     */

//    catch(QWHO_CMD->main(); publish);
//    write("\n");

    /*
     *
     */
    login_data[I_TRIES]=0;
    input_to("get_name", INPUT_PROMPT | INPUT_IGNORE_BANG, NAME_PROMPT);
}