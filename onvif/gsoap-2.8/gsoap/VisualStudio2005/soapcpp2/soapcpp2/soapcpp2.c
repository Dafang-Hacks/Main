/*
	soapcpp2.c

	Main compiler and code generator batch program.

gSOAP XML Web services tools
Copyright (C) 2000-2013, Robert van Engelen, Genivia Inc. All Rights Reserved.
This part of the software is released under one of the following licenses:
GPL or Genivia's license for commercial use.
--------------------------------------------------------------------------------
GPL license.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

Author contact information:
engelen@genivia.com / engelen@acm.org

This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia, Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#include "soapcpp2.h"

#ifndef SOAPCPP2_IMPORT_PATH
#define SOAPCPP2_IMPORT_PATH (NULL)
#endif

extern void init(void);
extern int yyparse(void);
extern FILE *yyin;

extern char *ns_cname(char*, char*);

FILE *fmsg;		/* fd to flush compiler messages */

int vflag = 0;		/* SOAP version, -1=no SOAP, 0=not set, 1=1.1, 2=1.2 */
int wflag = 0;		/* when set, don't generate WSDL and schema files */
int Cflag = 0;		/* when set, generate only files for clients */
int cflag = 0;		/* when set, generate files with .c extension */
int aflag = 0;		/* when set, use value of SOAP Action to dispatch method at server side */
int Aflag = 0;		/* when set, require SOAP Action to dispatch method at server side */
int bflag = 0;		/* when set, serialize byte arrays char[N] as string */
int eflag = 0;		/* when set, use SOAP RPC encoding by default */
unsigned long fflag = 0;/* multi-file split for each bundle of -fN defs */
int iflag = 0;		/* when set, generate new style proxy/object classes inherited from soap struct */
int jflag = 0;		/* when set, generate new style proxy/object classes */
int kflag = 0;		/* when set, generate data traversal/walker routines */
int mflag = 0;		/* when set, generate code that requires array/binary classes to explicitly remove malloced array */
int nflag = 0;		/* when set, names the namespaces global struct '%NAME%_namespaces */
int lflag = 0;		/* when set, create library */
int Lflag = 0;		/* when set, don't generate soapClientLib/soapServerLib */
int Qflag = 0;		/* when set, use C++ namespaces for custom serializers */
int sflag = 0;		/* when set, generate strict validation checks */
int Sflag = 0;		/* when set, generate only files for servers */
int Tflag = 0;		/* when set, generates server auto-test code */
int tflag = 0;		/* when set, generates typed messsages (with xsi:type attributes) */
int uflag = 0;		/* when set, uncomment WSDL and schema output */
int xflag = 0;		/* when set, don't generate sample XML message files */
int yflag = 0;		/* when set, add C/C++ info in sample XML messages */
int zflag = 0;		/* when set, use backward compatibility option */

int stop_flag = 0;

char dirpath[1024];	/* directory path for generated source files */
char *prefix = "soap";	/* file name prefix for generated source files */
char filename[1024];	/* current file name */
char *importpath = SOAPCPP2_IMPORT_PATH; /* default file import path */

/*
IMPORTANT:

The terms and conditions of use of this software do not allow for the removal
of the copyright notice from the main program for visual display. For
integrations with other software, a similar copyright notice must be produced
that is visible to users of the software.
*/

int
main(int argc, char **argv)
{	int i, g;
	char *a, *s;
	fmsg = stderr;
	strcpy(filename, "<stdin>");
	for (i = 1; i < argc; i++)
	{	a = argv[i];
		if (*a == '-'
#ifdef WIN32
		 || *a == '/'
#endif
		)
		{	g = 1;
			while (g && *++a)
				switch (*a)
				{	case 'C':
						Cflag = 1;
						if (Sflag)
            					  fprintf(stderr, "soapcpp2: using both options -C and -S omits client/server code\n");
						break;
					case 'c':
						cflag = 1;
						break;
					case 'd':
						a++;
						g = 0;
						if (*a)
							strcpy(dirpath, a);
						else if (i < argc && argv[++i])
							strcpy(dirpath, argv[i]);
						else
							execerror("Option -d requires a directory path");
						if (*dirpath && dirpath[strlen(dirpath)-1] != '/' && dirpath[strlen(dirpath)-1] != '\\')
							strcat(dirpath, SOAP_PATHCAT);
						break;
					case 'e':
						eflag = 1;
						break;
					case 'f':
						a++;
						g = 0;
						if (*a)
							fflag = strtoul(a, NULL, 10);
						else if (i < argc && argv[++i])
							fflag = strtoul(argv[i], NULL, 10);
						if (!fflag)
							execerror("Option -f requires a value");
						if (fflag < 10)
						  fflag = 10;
						break;
					case 'a':
						aflag = 1;
						break;
					case 'A':
						aflag = 1;
						Aflag = 1;
						break;
					case 'b':
						bflag = 1;
						break;
					case '?':
					case 'h':
						fprintf(stderr, "Usage: soapcpp2 [-0|-1|-2] [-C|-S] [-T] [-L] [-a] [-A] [-b] [-c] [-d path] [-e] [-f N] [-h] [-i] [-I path"SOAP_PATHSEP"path"SOAP_PATHSEP"...] [-k] [-l] [-m] [-n] [-p name] [-s] [-t] [-u] [-v] [-w] [-x] [-y] [-z#] [infile]\n\n");
						fprintf(stderr, "\
-1      generate SOAP 1.1 bindings\n\
-2      generate SOAP 1.2 bindings\n\
-0      remove SOAP bindings, use REST\n\
-C	generate client-side code only\n\
-S	generate server-side code only\n\
-T	generate server auto-test code\n\
-L	don't generate soapClientLib/soapServerLib\n\
-a	use SOAPAction with WS-Addressing to invoke server-side operations\n\
-A	require SOAPAction to invoke server-side operations\n\
-b	serialize byte arrays char[N] as string\n\
-c      generate C source code\n\
-dpath  use path to save files\n\
-e	generate SOAP RPC encoding style bindings\n\
-fN	file split of N XML serializer implementations per file (N>=10)\n\
-h	display help info\n\
-Ipath  use path(s) for #import (paths separated with '"SOAP_PATHSEP"')\n\
-i      generate C++ service proxies and objects inherited from soap struct\n\
-j      generate C++ service proxies and objects that share a soap struct\n\
-k      generate data structure walkers (experimental)\n\
-l      generate linkable modules (experimental)\n\
-m      generate Matlab(tm) code for MEX compiler\n\
-n      use service name to rename service functions and namespace table\n\
-pname  save files with new prefix name instead of 'soap'\n\
-Qname  use name as the C++ namespace for decls, including custom serializers\n\
-qname  use name as the C++ namespace for decls, excluding custom serializers\n\
-s      generate deserialization code with strict XML validation checks\n\
-t      generate code for fully xsi:type typed SOAP/XML messaging\n\
-u	uncomment comments in WSDL/schema output by suppressing XML comments\n\
-v	display version info\n\
-w	don't generate WSDL and schema files\n\
-x	don't generate sample XML message files\n\
-y	include C/C++ type access information in sample XML messages\n\
-z1	generate deprecated old-style C++ service proxies and objects\n\
infile	header file to parse (or stdin)\n\
\n");
						exit(0);
					case 'I':
						a++;
						g = 0;
						s = NULL;
						if (*a)
							s = a;
						else if (i < argc && argv[++i])
							s = argv[i];
						else
							execerror("Option -I requires an import path");
						if (importpath && s)
						{	char *t	= emalloc(strlen(importpath) + strlen(s) + 2);
							strcpy(t, importpath);
							strcat(t, SOAP_PATHSEP);
							strcat(t, s);
							importpath = t;
						}
						else
							importpath = s;
						break;
					case 'i':
						iflag = 1;
						break;
					case 'j':
						jflag = 1;
						break;
					case 'k':
						kflag = 1;
						break;
					case 'm':
						mflag = 1;
						break;
					case 'n':
						nflag = 1;
						break;
					case 'l':
						lflag = 1;
						break;
					case 'L':
						Lflag = 1;
						break;
					case 's':
						sflag = 1;
						break;
					case 'S':
						Sflag = 1;
						if (Cflag)
            					  fprintf(stderr, "soapcpp2: using both options -C and -S omits client/server code\n");
						break;
					case 'T':
						Tflag = 1;
						break;
					case 't':
						tflag = 1;
						break;
					case 'u':
						uflag = 1;
						break;
					case 'w':
						wflag = 1;
						break;
					case 'x':
						xflag = 1;
						break;
					case 'y':
						yflag = 1;
						break;
					case 'p':
						a++;
						g = 0;
						if (*a)
							prefix = ns_cname(a, NULL);
						else if (i < argc && argv[++i])
							prefix = ns_cname(argv[i], NULL);
						else
							execerror("Option -p requires an output file name prefix");
						break;
					case 'Q':
						Qflag = 1;
						/* fall through */
					case 'q':
						a++;
						g = 0;
						if (*a)
							namespaceid = ns_cname(a, NULL);
						else if (i < argc && argv[++i])
							namespaceid = ns_cname(argv[i], NULL);
						else
							execerror("Option -q requires a namespace name");
						break;
					case '0':
						vflag = -1;
						break;
					case '1':
						vflag = 1;
						envURI = "http://schemas.xmlsoap.org/soap/envelope/";
						encURI = "http://schemas.xmlsoap.org/soap/encoding/";
						break;
					case '2':
						vflag = 2;
						envURI = "http://www.w3.org/2003/05/soap-envelope";
						encURI = "http://www.w3.org/2003/05/soap-encoding";
						rpcURI = "http://www.w3.org/2003/05/soap-rpc";
						break;
					case 'v':
						stop_flag = 1;
						break;
					case 'z':
						a++;
						g = 0;
						if (*a)
							zflag = *a - '0';
						else if (i < argc && argv[++i])
							zflag = *argv[i] - '0';
						else
							execerror("Option -z requires a digit");
						break;
					default:
            					fprintf(stderr, "soapcpp2: Unknown option %s\n", a);
            					exit(1);
				}
		}
		else if (!(yyin = fopen(argv[i], "r")))
		{	sprintf(errbuf, "Cannot open file \"%s\" for reading", argv[i]);
			execerror(errbuf);
		}
		else
			strcpy(filename, argv[i]);
	}
	fprintf(fmsg, "\n**  The gSOAP code generator for C and C++, soapcpp2 release "VERSION"\n**  Copyright (C) 2000-2013, Robert van Engelen, Genivia Inc.\n**  All Rights Reserved. This product is provided \"as is\", without any warranty.\n**  The soapcpp2 tool is released under one of the following licenses:\n**  GPL or the commercial license by Genivia Inc.\n\n");
	if (stop_flag)
	  exit(0);
	init();
	if (yyparse())
		synerror("skipping the remaining part of the input");
	return errstat();
}
