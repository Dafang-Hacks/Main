/*
	httpform.h

	gSOAP HTTP POST application/x-www-form-urlencoded plugin.

	Requires linkage with httpget.c (for query_key and query_val)

Note: multipart/related and multipart/form-data are already handled in gSOAP.

gSOAP XML Web services tools
Copyright (C) 2004-2005, Robert van Engelen, Genivia, Inc. All Rights Reserved.

--------------------------------------------------------------------------------
gSOAP public license.

The contents of this file are subject to the gSOAP Public License Version 1.3
(the "License"); you may not use this file except in compliance with the
License. You may obtain a copy of the License at
http://www.cs.fsu.edu/~engelen/soaplicense.html
Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the License.

The Initial Developer of the Original Code is Robert A. van Engelen.
Copyright (C) 2000-2004 Robert A. van Engelen, Genivia inc. All Rights Reserved.
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
*/

#ifndef HTTPFORM_H
#define HTTPFORM_H

#include "stdsoap2.h"
#include "httpget.h"	/* require soap_decode_string */

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP_FORM_ID "HTTP-FORM-1.1" /* plugin identification */

extern const char http_form_id[];

/* This is the local plugin data shared among all copies of the soap struct: */
struct http_form_data
{ int (*fparsehdr)(struct soap*, const char*, const char*); /* to save and call the internal HTTP header parser */
  int (*handler)(struct soap*);
};

int http_form(struct soap*, struct soap_plugin*, void*);

char *form(struct soap*);

#ifdef __cplusplus
}
#endif

#endif
