/*
 * Master Object Prototypes
 * http://abathur.github.io/ldmud-doc/build/html/master/overview.html
 *
 */
#ifndef _H_MASTER_PROTO_H_
#define _H_MASTER_PROTO_H_

// master_init.c

void      inaugurate_master(int arg);
string    get_master_uid();
void      flag(string arg);
string*   epilog(int eflag);
void      preload(string file);
void      external_master_reload();
void      reactivate_destructed_master(int removed);
string    get_simul_efun();

// connect.c

object    connect();
void      disconnect(object ob);
void      remove_player(object victim);
void      stale_erq(closure callback);

// runtime.c

object    compile_object(string filename);
mixed     include_file(string file, string compiled_file, int sys_include);
mixed     inherit_file(string file, string compiled_file);
string    get_wiz_name(string file);
string    printf_obj_name(object ob);
mixed     prepare_destruct(object ob);
void      quota_demon();
void      receive_udp(string host, string msg, int hostport);
void      slow_shut_down(int minutes);
void      notify_shutdown();
int       handle_external_signal(int signal);
void      low_memory(int what, int limitvalue, int memory, int reservestate);

// error.c

void      dangling_lfun_closure();
void      log_error(string file, string err, int warn);
mixed     heart_beat_error(object culprit, string err, string prg, string curobj, int line);
void      runtime_error(string err, string prg, string curobj, int line);
void      runtime_warning(string msg, string curobj, string prg, int line);

// permissions.c

int       privilege_violation(string op, mixed who, mixed arg3, mixed arg4);
int       query_allow_shadow(object victim);
int       valid_exec(string name);
int       valid_query_snoop(object obj);
int       valid_snoop(object snoopee, object snooper);
int       valid_trace(string what, mixed arg);

// valid.c

string    get_bb_uid();
int       valid_seteuid(object ob, string neweuid);
mixed     valid_read(string path, string euid, string fun, object caller);
mixed     valid_write(string path, string euid, string fun, object caller);

// ed.c

string    make_path_absolute(string str);
int       save_ed_setup(object who, int code);
int       retrieve_ed_setup(object who);
string    get_ed_buffer_save_file_name(string file);

#endif // _H_MASTER_PROTO_H_

