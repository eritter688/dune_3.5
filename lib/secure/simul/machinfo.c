/*
**  secure/simul/machinfo.c
**
**  Machine Info.
**
**  Grab some information from the MUD driver's permanent defines (see lex.c)
**  This allows to use efuns to use the defines (and such not use the silly names)
**  Iria@Dune.
*/

string version()                {   return  __VERSION__;         }
string query_host_ip_number()   {   return __HOST_IP_NUMBER__;   }
string query_host_name()        {   return __HOST_NAME__;        }
string query_domain_name()      {   return __DOMAIN_NAME__;      }
string query_host_fqdn()        {
    return sprintf("%s%c%s",
        strlen(query_host_name())?query_host_name():"localhost",
        strlen(query_domain_name())?'.':0,
        strlen(query_domain_name())?query_domain_name():""
    );
}

#ifdef __LDMUD__
// LDmud extensions

int    version_major()          {   return __VERSION_MAJOR__;        }
int    version_minor()          {   return __VERSION_MINOR__;        }
int    version_micro()          {   return __VERSION_MICRO__;        }
int    version_patch()          {   return __VERSION_PATCH__;        }
status version_is_release()     {   return __VERSION_PATCH__==0;     }

#else

#define __GRAB_SUBVER__(maj,min,mic)         sscanf(__VERSION__,"%d.%d.%d",maj,min,mic)

int version_major() {
        int maj,min,mic;
        __GRAB_SUBVER__(maj,min,mic);
        return maj;
}

int version_minor() {
        int maj,min,mic;
        __GRAB_SUBVER__(maj,min,mic);
        return min;
}

int version_micro() {
        int maj,min,mic;
        __GRAB_SUBVER__(maj,min,mic);
        return mic;
}

int version_patch() {
        int dev;
        sscanf(__VERSION__,"%~s@%d",dev);
        return dev;
}

status version_is_release()     {   return 1;   }

#endif /* __LDMUD__ */
