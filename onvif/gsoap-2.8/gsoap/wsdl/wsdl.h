/*
	wsdl.h

	WSDL 1.1 and WSDL 2.0 schemas

--------------------------------------------------------------------------------
gSOAP XML Web services tools
Copyright (C) 2000-2013, Robert van Engelen, Genivia Inc. All Rights Reserved.
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

//gsoapopt w

//gsoap wsdl schema documentation:	WSDL 1.1/2.0 binding schema
//gsoap wsdl schema namespace:		http://schemas.xmlsoap.org/wsdl/
//gsoap wsdl schema namespace2:		http://www.w3.org/ns/wsdl
//gsoap wsdl schema elementForm:	qualified
//gsoap wsdl schema attributeForm:	unqualified

#import "imports.h"
#import "schema.h"
#import "soap.h"
#import "mime.h"
#import "dime.h"
#import "http.h"
#import "gwsdl.h"
#import "wsam.h"
#import "wsp.h"

class wsdl__definitions;		// forward declaration

class wsdl__import
{ public:
	@xsd__anyURI			namespace_;
	@xsd__anyURI			location;
  private:
  	wsdl__definitions		*definitionsRef;
  public:
					wsdl__import();
	int				preprocess(wsdl__definitions&);
  	int				traverse(wsdl__definitions&);
	void				definitionsPtr(wsdl__definitions*);
	wsdl__definitions		*definitionsPtr() const;
};

class wsdl__types : public xs__schema				// WSDL 2.0 <types> inlined schema
{ public:
	xsd__string			documentation;		// <wsdl:documentation>?
	std::vector<xs__schema*>	xs__schema_; 		// <xs:schema>*
  public:
	int				preprocess(wsdl__definitions&);
	int				traverse(wsdl__definitions&);
};

class wsdl__part
{ public:
	@xsd__NMTOKEN			name;
	@xsd__QName			element;
	@xsd__QName			type;
	xsd__string			documentation;		// <wsdl:documentation>?
  private:
  	xs__element			*elementRef;		// traverse() finds element
  	xs__simpleType			*simpleTypeRef;		// traverse() finds simpleType
  	xs__complexType			*complexTypeRef;	// traverse() finds complexType
  public:
					wsdl__part();
  	int				traverse(wsdl__definitions&);
	void				elementPtr(xs__element*);
	void				simpleTypePtr(xs__simpleType*);
	void				complexTypePtr(xs__complexType*);
	xs__element			*elementPtr() const;
	xs__simpleType			*simpleTypePtr() const;
	xs__complexType			*complexTypePtr() const;
};

class wsdl__message
{ public:
	@xsd__NMTOKEN			name;
	xsd__string			documentation;		// <wsdl:documentation>?
	std::vector<wsp__Policy>	wsp__Policy_;		// <wsp:Policy>*
	std::vector<wsp__PolicyReference> wsp__PolicyReference_;// <wsp:PolicyReference>*
	std::vector<wsdl__part>		part;			// <wsdl:part>*
  public:
  	int				traverse(wsdl__definitions&);
};

class wsdl__ioput
{ public:
	@xsd__NMTOKEN			name;
	@xsd__QName			message;
	@xsd__NMTOKEN			messageLabel;		// WSDL 2.0
	@xsd__QName			element;		// WSDL 2.0
	@xsd__anyURI			wsa__Action;
	@xsd__anyURI			wsam__Action;
	xsd__string			documentation;		// <wsdl:documentation>?
	wsp__Policy			*wsp__Policy_;		// <wsp:Policy>?
	wsp__PolicyReference		*wsp__PolicyReference_;	// <wsp:PolicyReference>?
  private:
  	wsdl__message			*messageRef;		// traverse() finds message
  	xs__element			*elementRef;		// traverse() finds element WSDL 2.0
  public:
  					wsdl__ioput();
  	int				traverse(wsdl__definitions&);
	void				messagePtr(wsdl__message*);
	wsdl__message			*messagePtr() const;
	void				elementPtr(xs__element*);
	xs__element			*elementPtr() const;
};

class wsdl__fault
{ public:
	@xsd__NMTOKEN			name;
	@xsd__QName			message;
	@xsd__QName			ref;			// WSDL 2.0
	@xsd__NMTOKEN			messageLabel;		// WSDL 2.0
	@xsd__QName			element;		// WSDL 2.0
	@xsd__anyURI			wsa__Action;
	@xsd__anyURI			wsam__Action;
	xsd__string			documentation;		// <wsdl:documentation>?
	wsp__Policy			*wsp__Policy_;		// <wsp:Policy>?
	wsp__PolicyReference		*wsp__PolicyReference_;	// <wsp:PolicyReference>?
  private:
  	wsdl__message			*messageRef;		// traverse() finds message
  	xs__element			*elementRef;		// traverse() finds element WSDL 2.0
  public:
  					wsdl__fault();
  	int				traverse(wsdl__definitions&);
	void				messagePtr(wsdl__message*);
	wsdl__message			*messagePtr() const;
	void				elementPtr(xs__element*);
	xs__element			*elementPtr() const;
};

class wsdl__operation
{ public:
	@xsd__NMTOKEN			name;
	@xsd__anyURI			pattern;		// WSDL 2.0
	@xsd__anyURI			style;			// WSDL 2.0
	@xsd__string			wrpc__signature;	// WSDL 2.0
	@xsd__string			parameterOrder;
	xsd__string			documentation;		// <wsdl:documentation>?
	wsp__Policy			*wsp__Policy_;		// <wsp:Policy>?
	wsp__PolicyReference		*wsp__PolicyReference_;	// <wsp:PolicyReference>?
	$int				__union1;
	union wsdl__union_ioput					// <wsdl:input>|<wsdl:output>
	{	wsdl__ioput		*input;
		wsdl__ioput		*output;
	}				__ioput1;
	$int				__union2;
	union wsdl__union_ioput 	__ioput2;		// <wsdl:input>|<wsdl:output>
	std::vector<wsdl__fault>	fault;			// <wsdl:fault>*
	std::vector<wsdl__fault>	infault;		// <wsdl:infault>* WSDL 2.0
	std::vector<wsdl__fault>	outfault;		// <wsdl:outfault>* WSDL 2.0
  public:
  	int				traverse(wsdl__definitions&);
};

class wsdl__portType			// ... and WSDL 2.0 interface
{ public:
	@xsd__NMTOKEN			name;
	@xsd__QName			extends;		// WSDL 2.0
	@xsd__anyURI			styleDefault;		// WSDL 2.0
	xsd__string			documentation;		// <wsdl:documentation>?
	wsp__Policy			*wsp__Policy_;		// <wsp:Policy>?
	wsp__PolicyReference		*wsp__PolicyReference_;	// <wsp:PolicyReference>?
	std::vector<wsdl__fault>	fault;			// <wsdl:fault>* WSDL 2.0
	std::vector<wsdl__operation>	operation;		// <wsdl:operation>*
  private:
	wsdl__definitions		*definitionsRef;
  public:
					wsdl__portType();
  	int				traverse(wsdl__definitions&);
	void				definitionsPtr(wsdl__definitions*);
	wsdl__definitions		*definitionsPtr() const;
};	

class wsdl__ext_ioput			// binding extensibility element
{ public:
	@xsd__NMTOKEN			name;
	@xsd__NMTOKEN			messageLabel;		// WSDL 2.0
	xsd__string			documentation;		// <wsdl:documentation>?
	wsp__Policy			*wsp__Policy_;		// <wsp:Policy>?
	wsp__PolicyReference		*wsp__PolicyReference_;	// <wsp:PolicyReference>?
	dime__message			*dime__message_;	// <dime:message>?
	soap__body			*soap__body_;		// <soap:body>?
	xsd__string			http__urlEncoded;	// <http:urlEncoded>?
	mime__multipartRelated		*mime__multipartRelated_;// <mime:multipartRelated>?
	mime__content			*mime__content_;	// <mime:content>?
	mime__mimeXml			*mime__mimeXml_;	// <mime:mimeXml>?
	std::vector<soap__header>	soap__header_;		// <soap:header>*
	std::vector<wsoap__module>	wsoap__module_;		// <wsoap:module>* WSDL 2.0
	std::vector<wsoap__header>	wsoap__header_;		// <wsoap:header>* WSDL 2.0
	std::vector<whttp__header>	whttp__header_;		// <whttp:header>* WSDL 2.0
  public:
  	int				traverse(wsdl__definitions&);
};

class wsdl__ext_fault			// binding extensibility element
{ public:
	@xsd__NMTOKEN			name;
	@xsd__QName			ref;			// WSDL 2.0
	@xsd__NMTOKEN			messageLabel;		// WSDL 2.0
	@xsd__QName			wsoap__code;		// WSDL 2.0
	@xsd__QName			wsoap__subcodes;	// WSDL 2.0
	xsd__string			documentation;		// <wsdl:documentation>?
	wsp__Policy			*wsp__Policy_;		// <wsp:Policy>?
	wsp__PolicyReference		*wsp__PolicyReference_;	// <wsp:PolicyReference>?
	soap__fault			*soap__fault_;		// <soap:fault>?
	std::vector<wsoap__module>	wsoap__module_;		// <wsoap:module>* WSDL 2.0
  private:
	wsdl__fault			*faultRef;
  public:
  					wsdl__ext_fault();
  	int				traverse(wsdl__definitions&, wsdl__portType*);
	void				faultPtr(wsdl__fault*);
	wsdl__fault			*faultPtr() const;
};

class wsdl__ext_operation		// binding extensibility element
{ public:
	@xsd__NMTOKEN			name;
	@xsd__QName			ref;			// WSDL 2.0
	@xsd__anyURI			wsoap__mep;		// WSDL 2.0
	@xsd__anyURI			wsoap__action;		// WSDL 2.0
	@xsd__string			whttp__method;		// WSDL 2.0
	@xsd__string			whttp__location;	// WSDL 2.0
	xsd__string			documentation;		// <wsdl:documentation>?
	wsp__Policy			*wsp__Policy_;		// <wsp:Policy>?
	wsp__PolicyReference		*wsp__PolicyReference_;	// <wsp:PolicyReference>?
	soap__operation			*soap__operation_;	// <soap:operation>?
	http__operation			*http__operation_;	// <http:operation>?
	wsdl__ext_ioput			*input;			// <wsdl:input>?
	wsdl__ext_ioput			*output;		// <wsdl:output>?
	std::vector<wsdl__ext_fault>	fault;			// <wsdl:fault>*
	std::vector<wsdl__ext_fault>	infault;		// <wsdl:infault>* WSDL 2.0
	std::vector<wsdl__ext_fault>	outfault;		// <wsdl:outfault>* WSDL 2.0
  private:
  	wsdl__operation			*operationRef;		// traverse() finds operation
  public:
  					wsdl__ext_operation();
  	int				traverse(wsdl__definitions&, wsdl__portType*);
	void				operationPtr(wsdl__operation*);
	wsdl__operation			*operationPtr() const;
};

class wsdl__binding
{ public:
	@xsd__NMTOKEN			name;
	@xsd__QName			type;
	@xsd__anyURI			type_;			// WSDL 2.0
	@xsd__QName			interface_;		// WSDL 2.0
	@xsd__boolean			whttp__cookies = false;	// WSDL 2.0
	@xsd__string			wsoap__version;		// WSDL 2.0
	@xsd__anyURI			wsoap__protocol;	// WSDL 2.0
	@xsd__anyURI			wsoap__mepDefault;	// WSDL 2.0
	@xsd__string			whttp__methodDefault;	// WSDL 2.0
	xsd__string			documentation;		// <wsdl:documentation>?
	std::vector<wsp__Policy>	wsp__Policy_;		// <wsp:Policy>*
	std::vector<wsp__PolicyReference> wsp__PolicyReference_;// <wsp:PolicyReference>*
	soap__binding			*soap__binding_;	// <soap:binding>?
	http__binding			*http__binding_;	// <http:binding>?
	std::vector<wsoap__module>	wsoap__module_;		// <wsoap:module>* WSDL 2.0
	std::vector<wsdl__ext_fault>	fault;			// <wsdl:fault>* WSDL 2.0
	std::vector<wsdl__ext_operation> operation;		// <wsdl:operation>*
  private:
	wsdl__portType			*portTypeRef;		// traverse() finds portType/interface
  public:
					wsdl__binding();
	int				traverse(wsdl__definitions&);
	void				portTypePtr(wsdl__portType*);
	wsdl__portType			*portTypePtr() const;
};

class wsdl__port			// ... and WSDL 2.0 endpoint
{ public:
	@xsd__NMTOKEN			name;
	@xsd__QName			binding;
	@xsd__anyURI			address;		// WSDL 2.0
	@xsd__NMTOKEN			whttp__authenticationScheme; // WSDL 2.0
	@xsd__NMTOKEN			whttp__authenticationRealm; // WSDL 2.0
	xsd__string			documentation;		// <wsdl:documentation>?
	wsp__Policy			*wsp__Policy_;		// <wsp:Policy>?
	wsp__PolicyReference		*wsp__PolicyReference_;	// <wsp:PolicyReference>?
	wsa__EndpointReferenceType	*wsa__EndpointReference;// <wsa:EndpointReference>?
	soap__address			*soap__address_;	// <soap:address>?
	http__address			*http__address_;	// <http:address>?
  private:
	wsdl__binding			*bindingRef;		// traverse() finds binding
  public:
  					wsdl__port();
	int				traverse(wsdl__definitions&);
	void				bindingPtr(wsdl__binding*);
	wsdl__binding			*bindingPtr() const;
};

class wsdl__service
{ public:
	@xsd__NMTOKEN			name;
	@xsd__QName			interface_;		// WSDL 2.0
	xsd__string			documentation;		// <wsdl:documentation>?
	std::vector<wsp__Policy>	wsp__Policy_;		// <wsp:Policy>*
	std::vector<wsp__PolicyReference> wsp__PolicyReference_;// <wsp:PolicyReference>*
	std::vector<wsdl__port>		port;			// <wsdl:port>* WSDL 1.1
	std::vector<wsdl__port>		endpoint;		// <wsdl:port>* WSDL 2.0
  public:
	int				traverse(wsdl__definitions&);
};

class wsdl__definitions
{ public:
	@xsd__NMTOKEN			name;
	@xsd__anyURI			targetNamespace		= "";
	@xsd__NMTOKEN			version;
	std::vector<wsdl__import>	import;			// <wsdl:import>*
	xsd__string			documentation;		// <wsdl:documentation>?
	xsd__string			wsp__UsingPolicy;	// <wsp:UsingPolcy>?
	std::vector<wsp__Policy>	wsp__Policy_;		// <wsp:Policy>*
	wsdl__types			*types;			// <wsdl:types>?
	std::vector<wsdl__message>	message;		// <wsdl:message>*
	std::vector<wsdl__portType>	portType;		// <wsdl:portType>* WSDL 1.1
	std::vector<wsdl__portType>	interface_;		// <wsdl:interface>* WSDL 2.0
	std::vector<wsdl__binding>	binding;		// <wsdl:binding>*
	std::vector<wsdl__service>	service;		// <wsdl:service>*
	std::vector<gwsdl__portType>	gwsdl__portType_;	// <gwsdl:portType>* For the moment, we will hardcode this which makes it easier to access. WSDL 1.1 does not allow this to be extended anyway
	struct soap			*soap;
  private:
	bool				soap12;
	bool				updated;
	char*				location;
	int				redirs;
	SetOfString			builtinTypeSet;
	SetOfString			builtinElementSet;
	SetOfString			builtinAttributeSet;
  public:
					wsdl__definitions();
					wsdl__definitions(struct soap*, const char*, const char*);
	virtual				~wsdl__definitions();
	int				get(struct soap*);	// gSOAP getter is triggered after parsing
	int				preprocess();
	int				traverse();
	int				read(int, char**);
	int				read(const char *cwd, const char*);
	const char*			sourceLocation();
	int				error();
	void				print_fault();
	void				builtinType(const char*);
	void				builtinTypes(const SetOfString&);
	void				builtinElement(const char*);
	void				builtinElements(const SetOfString&);
	void				builtinAttribute(const char*);
	void				builtinAttributes(const SetOfString&);
	const SetOfString&		builtinTypes() const;
	const SetOfString&		builtinElements() const;
	const SetOfString&		builtinAttributes() const;
	friend ostream&			operator<<(ostream&, const wsdl__definitions&);
	friend istream&			operator>>(istream&, wsdl__definitions&);
};

extern ostream &operator<<(ostream &o, const wsdl__definitions &e);
extern istream &operator>>(istream &i, wsdl__definitions &e);
