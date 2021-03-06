/*
 * object find_player(string name)
 * Get the player object associated to the name.
 *
 * object find_living(string name)
 *
 */
#include <daemons.h>

/*
 * Get the player object associated to the name.
 */
object find_player(string name) {
    return LIVINGSD->get_living(name, 0);
}

/*
 * Get the first non-interactive living with that name.
 * Never returns a player object.
 */
object find_npc(string name) {
    return LIVINGSD->get_living(name, 1);
}

object find_living(string name) {
    object ob;
    if(!(ob=LIVINGSD->get_living(name, 0)))
        ob=LIVINGSD->get_living(name, 1);
    return ob;
}

protected void set_living_name(string name) {
    LIVINGSD->add_living(name, previous_object());
}

object* find_livings(string name) {
    object* ob;
    return LIVINGSD->get_livings(name);
}
