#ifndef DROPBEAR_LOCAL_OPTIONS_H_
#define DROPBEAR_LOCAL_OPTIONS_H_

/* Default hostkey paths - these can be specified on the command line */
#define DSS_PRIV_FILENAME "/system/sdcard/config/dropbear_dss_host_key"
#define RSA_PRIV_FILENAME "/system/sdcard/config/dropbear_rsa_host_key"
#define ECDSA_PRIV_FILENAME "/system/sdcard/config/dropbear_ecdsa_host_key"
#define ED25519_PRIV_FILENAME "/system/sdcard/config/dropbear_ed25519_host_key"

/* The default path. This will often get replaced by the shell */
#define DEFAULT_PATH "/usr/bin:/bin:/system/bin:/system/sdcard/bin/"

#endif /* DROPBEAR_LOCAL_OPTIONS_H_ */
