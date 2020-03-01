This directory contains plugins for extending the gSOAP engine's capabilities.

cacerts.*	To embed client-side certificate chain in code (no cacerts.pem)
httpda.*	HTTP Digest Authentication, see doc/httpda
httpget.*	HTTP GET method support
httpform.*	HTTP POST application/x-www-form-urlencoded support
httppost.*	HTTP POST method support for example non-SOAP requests
httpmd5.*	HTTP MD5 checksums
logging.*	Message logging (selective inbound/outbound)
mq.*		Message queueing (for WS-RM message queueing support)
plugin.*	Example plugin for aspiring plugin developers
wsaapi.*	WS-Addressing (wsa) plugin, see doc/wsa
wsrmapi.*	WS-ReliableMessaging (wsrm) plugin, see doc/wsrm
wsseapi.*	WS-Security (wsse) plugin, see doc/wsse
wsddapi.*	WS-Discovery (wsdd) library, see doc/wsdd

Supporting modules:

smdevp.*	EVP-based streaming (signed) message digest engine
mecevp.*	EVP-based streaming message encryption engine
md5evp.*	MD5 EVP interface for httpda and httpmd5
threads.*	Portable threads, mutex, and condition variables

Test programs for plugins:

httpdatest.*	HTTP Digest Athentication test and example code
httpgettest.*	HTTP GET test and example code
httpposttest.*	HTTP POST test and example code
httpmd5test.*	HTTP MD5 integrity protection test and example code

C versus C++ usage:

All files are suitable for C and C++ compilation and C++ checks are included in
the source code whenever applicable. If necessary, rename the .c file(s) to
.cpp for your project.

IMPORTANT FOR VISUAL STUDIO USERS: make sure to compile all sources in C++
compilation mode. If you migrate to a project file .vcproj, please set
CompileAs="2" in your .vcproj file.
