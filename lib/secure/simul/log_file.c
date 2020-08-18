/*
**  secure/simul/log_file.c
**
**  void log_file(string file,string message)
**  Iria@Dune.
**  Ported to DUNE_3.5. - Math
*/
#define LOG(x)              sprintf("/log/%s",x)
#define SECURELOG(x)        sprintf("/secure/log/%s",x[1..])
#define MAX_LOG_SIZE        50000
#define MAX_LOG_FILENAME    30

void log_file(string file,string str) {
    if(!stringp(file))
      raise_error("Bad argument 1 to log_file()\n");
    if(!stringp(str))
      raise_error("Bar argument 2 to log_file()\n");
    if(member(file,'/',1)!=-1 || file[0]=='.' || strlen(file)>MAX_LOG_FILENAME)
        raise_error(sprintf("Illegal file name to log_file(): %O\n",file));

    string log_file_name = (file[0]=='@' ? SECURELOG(file) : LOG(file));

    if(file_size(log_file_name)>MAX_LOG_SIZE)
        rename(log_file_name, sprintf("%s.old", log_file_name));
    write_file(log_file_name, sprintf("%s%c", str, str[<1]=='\n' ? 0 : '\n'));
}
