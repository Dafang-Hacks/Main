/*
	service.h

	Service structures

--------------------------------------------------------------------------------
gSOAP XML Web services tools
Copyright (C) 2001-2012, Robert van Engelen, Genivia Inc. All Rights Reserved.
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

#ifndef SERVICE_H
#define SERVICE_H

#include "includes.h"
#include "wsdlH.h"

class Message
{ public:
    const char *name;
    const char *URI;
    soap__styleChoice style;
    soap__useChoice use;
    const char *encodingStyle;
    const char *action;
    wsdl__message *message;
    xs__element *element;
    const char *body_parts;
    wsdl__part *part;
    bool mustUnderstand;
    vector<soap__header> header;
    vector<wsoap__header> wheader;
    mime__multipartRelated *multipartRelated;	// MIME
    mime__content *content;			// REST/MIME
    const char *layout;				// DIME
    const char *documentation;
    const char *ext_documentation;
    vector<const wsp__Policy*> policy;
    void generate(Types&, const char *sep, bool anonymous, bool remark, bool response, bool optional);
};

typedef map<const char*, Message*, ltstr> MapOfStringToMessage;

class Operation
{ public:
    const char *prefix;
    const char *URI;
    const char *name;
    const char *mep;			// WSDL 2.0
    const char *protocol;
    soap__styleChoice style;
    const char *parameterOrder;
    const char *action;
    const char *input_name;
    const char *output_name;
    Message *input; 			// name, use, and parts
    Message *output;			// name, use, and parts
    vector<Message*> infault;
    vector<Message*> outfault;
    const char *documentation;
    const char *operation_documentation;
    vector<const wsp__Policy*> policy;
    void generate(Types&, Service&);
};

class Service
{ public:
    const char *prefix;			// a gSOAP service has a unique namespace
    const char *URI;
    const char *name;			// binding name
    const char *type;			// portType
    const char *transport;		// binding transport
    SetOfString location;		// WSDL may specify multiple locations via <Port> -> <Binding>
    vector<Operation*> operation;
    MapOfStringToMessage header;
    MapOfStringToMessage headerfault;
    MapOfStringToMessage fault;
    MapOfStringToString service_documentation;
    MapOfStringToString port_documentation;
    MapOfStringToString binding_documentation;
    vector<const wsp__Policy*> policy;
    VectorOfString imports;
    Service();
    void generate(Types&);
    void add_import(const char*);
    void del_import(const char*);
};

typedef map<const char*, Service*, ltstr> MapOfStringToService;

class Definitions
{ public:
    Types types;				// to process schema type information
    MapOfStringToService services;		// service information gathered
    bool soap12;
    Definitions();
    void collect(const wsdl__definitions&);
    void compile(const wsdl__definitions&);
  private:
    void analyze(const wsdl__definitions&);
    void analyze_headers(const wsdl__definitions&, Service*, wsdl__ext_ioput*, wsdl__ext_ioput*);
    void analyze_faults(const wsdl__definitions&, Service*, Operation*, vector<wsdl__ext_operation>::const_iterator&);
    Message *analyze_fault(const wsdl__definitions&, Service*, const wsdl__ext_fault&);
    void generate();
};

#endif
