/*
	mime.cpp

	WSDL/MIME binding schema

--------------------------------------------------------------------------------
gSOAP XML Web services tools
Copyright (C) 2001-2008, Robert van Engelen, Genivia Inc. All Rights Reserved.
This software is released under one of the following licenses:
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
--------------------------------------------------------------------------------
A commercial use license is available from Genivia, Inc., contact@genivia.com
--------------------------------------------------------------------------------

*/

#include "wsdlH.h"		// cannot include "schemaH.h"
#include "includes.h"

extern const char *qname_token(const char*, const char*);

////////////////////////////////////////////////////////////////////////////////
//
//	mime:multipartRelated
//
////////////////////////////////////////////////////////////////////////////////

int mime__multipartRelated::traverse(wsdl__definitions& definitions)
{ if (vflag)
    cerr << "Analyzing mime multpartRelated " << endl;
  for (vector<mime__part>::iterator pt = part.begin(); pt != part.end(); ++pt)
    (*pt).traverse(definitions);
  return SOAP_OK;
}

////////////////////////////////////////////////////////////////////////////////
//
//	mime:part
//
////////////////////////////////////////////////////////////////////////////////

int mime__part::traverse(wsdl__definitions& definitions)
{ if (vflag)
    cerr << "Analyzing mime part " << endl;
  for (vector<soap__header>::iterator hd = soap__header_.begin(); hd != soap__header_.end(); ++hd)
    (*hd).traverse(definitions);
  return SOAP_OK;
}
