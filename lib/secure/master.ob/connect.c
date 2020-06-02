/*
 * Master Object - Connections
 *
 */

object connect() {

}

void disconnect(object ob) {

}

void remove_player(object victim) {
    catch(victim->quit());
    if(victim) {
        efun::destruct(victim);
    }
}

void stale_erq(closure callback) {

}