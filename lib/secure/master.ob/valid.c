/*
 * Master Object - Permissions
 *
 */

string get_bb_uid() {

}

int valid_seteuid(object ob, string neweuid) {
    return 1;
}

mixed valid_read(string path, string euid, string fun, object caller) {
    return 1;
}

mixed valid_write(string path, string euid, string fun, object caller) {
    return 1;
}
