/*
	varparam.h

	Example use of variable parameter lists with the full XML
	schema type hierarchy implemented as a C++ class hierarchy.

--------------------------------------------------------------------------------
gSOAP XML Web services tools
Copyright (C) 2001-2008, Robert van Engelen, Genivia, Inc. All Rights Reserved.
This software is released under one of the following two licenses:
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

//gsoap ns service name:	varparam
//gsoap ns service style:	rpc
//gsoap ns service encoding:	encoded
//gsoap ns service namespace:	urn:varparam
//gsoap ns service location:	http://websrv.cs.fsu.edu/~engelen/varparam.cgi

#include <iostream>
extern class std::ostream;	// transient (external) type

class xsd__anyType
{ char *__item;		// default is string, also to hold mixed-content when receiving typeless XML
  struct soap *soap;	// points to current gSOAP environment that created this object
  xsd__anyType();
  virtual ~xsd__anyType();
  virtual xsd__anyType*& operator[](int i);
  virtual void print(std::ostream &s) const;
};

class xsd__anySimpleType: xsd__anyType
{ xsd__anySimpleType();
  virtual ~xsd__anySimpleType();
  virtual void print(std::ostream &s) const;
};

class xsd__anyURI: xsd__anySimpleType
{ xsd__anyURI();
  virtual ~xsd__anyURI();
  xsd__anyURI(char *s);
  virtual void print(std::ostream &s) const;
}; 

class xsd__boolean: xsd__anySimpleType
{ bool __item;
  xsd__boolean();
  virtual ~xsd__boolean();
  xsd__boolean(bool b);
  virtual void print(std::ostream &s) const;
}; 

class xsd__date: xsd__anySimpleType	// requires "CCYY-MM-DD" string values
{ xsd__date();
  virtual ~xsd__date();
  xsd__date(char *s);
  virtual void print(std::ostream &s) const;
};

class xsd__dateTime: xsd__anySimpleType
{ time_t __item;		// remove time_t __item to use char*__item with "CCYY-MM-DDThh:mm:ssi" for dates outside the range 1902-2037
  xsd__dateTime();
  virtual ~xsd__dateTime();
  xsd__dateTime(time_t t);
  virtual void print(std::ostream &s) const;
};

class xsd__double: xsd__anySimpleType
{ double __item;
  xsd__double();
  virtual ~xsd__double();
  xsd__double(double d);
  virtual void print(std::ostream &s) const;
}; 

class xsd__duration: xsd__anySimpleType	// requires "PnYnMnDTnHnMnS" string values
{ xsd__duration();
  virtual ~xsd__duration();
  xsd__duration(char *s);
  virtual void print(std::ostream &s) const;
};

class xsd__float: xsd__anySimpleType
{ float __item;
  xsd__float();
  virtual ~xsd__float();
  xsd__float(float f);
  virtual void print(std::ostream &s) const;
}; 

class xsd__time: xsd__anySimpleType	// requires "hh:mm:ss" string values
{ xsd__time();
  virtual ~xsd__time();
  xsd__time(char *s);
  virtual void print(std::ostream &s) const;
};

class xsd__string: xsd__anySimpleType
{ xsd__string();
  xsd__string(char *s);
  virtual ~xsd__string();
  virtual void print(std::ostream &s) const;
}; 

class xsd__normalizedString: xsd__string	// requires strings without CR, LF, TAB
{ xsd__normalizedString();
  xsd__normalizedString(char *s);
  virtual ~xsd__normalizedString();
  virtual void print(std::ostream &s) const;
}; 

class xsd__token: xsd__normalizedString		// requires strings without CR, LF, TAB, no leading/trailing spaces, and no sequences of more than one space
{ xsd__token();
  xsd__token(char *s);
  virtual ~xsd__token();
  virtual void print(std::ostream &s) const;
}; 

class xsd__decimal: xsd__anySimpleType	// requires decimal as string values, can use double, but possible loss of precision
{ xsd__decimal();
  virtual ~xsd__decimal();
  xsd__decimal(char *s);
  virtual void print(std::ostream &s) const;
};

class xsd__integer: xsd__decimal	// requires integer as string values, can use loong long, but possible loss of data
{ xsd__integer();
  virtual ~xsd__integer();
  xsd__integer(char *s);
  virtual void print(std::ostream &s) const;
};

class xsd__nonPositiveInteger: xsd__integer	// requires non-positive integer as string values
{ xsd__nonPositiveInteger();
  virtual ~xsd__nonPositiveInteger();
  xsd__nonPositiveInteger(char *s);
  virtual void print(std::ostream &s) const;
}; 

class xsd__negativeInteger: xsd__nonPositiveInteger	// requires negative integer as string values
{ xsd__negativeInteger();
  virtual ~xsd__negativeInteger();
  xsd__negativeInteger(char *s);
  virtual void print(std::ostream &s) const;
}; 

class xsd__nonNegativeInteger: xsd__integer	// requires non-negative integer as string values
{ xsd__nonNegativeInteger();
  virtual ~xsd__nonNegativeInteger();
  xsd__nonNegativeInteger(char *s);
  virtual void print(std::ostream &s) const;
}; 

class xsd__positiveInteger: xsd__nonNegativeInteger	// requires positive integer as string values
{ xsd__positiveInteger();
  virtual ~xsd__positiveInteger();
  xsd__positiveInteger(char *s);
  virtual void print(std::ostream &s) const;
}; 

class xsd__long: xsd__integer
{ LONG64 __item;
  xsd__long();
  virtual ~xsd__long();
  xsd__long(LONG64 ll);
  virtual void print(std::ostream &s) const;
}; 

class xsd__int: xsd__long
{ int __item;
  xsd__int();
  virtual ~xsd__int();
  xsd__int(int i);
  virtual void print(std::ostream &s) const;
}; 

class xsd__short: xsd__int
{ short __item;
  xsd__short();
  virtual ~xsd__short();
  xsd__short(short h);
  virtual void print(std::ostream &s) const;
}; 

class xsd__byte: xsd__short
{ char __item;
  xsd__byte();
  virtual ~xsd__byte();
  xsd__byte(char c);
  virtual void print(std::ostream &s) const;
}; 

class xsd__unsignedLong: xsd__nonNegativeInteger
{ ULONG64 __item;
  xsd__unsignedLong();
  virtual ~xsd__unsignedLong();
  xsd__unsignedLong(ULONG64 ull);
  virtual void print(std::ostream &s) const;
}; 

class xsd__unsignedInt: xsd__unsignedLong
{ unsigned int __item;
  xsd__unsignedInt();
  virtual ~xsd__unsignedInt();
  xsd__unsignedInt(unsigned int ui);
  virtual void print(std::ostream &s) const;
}; 

class xsd__unsignedShort: xsd__unsignedInt
{ unsigned short __item;
  xsd__unsignedShort();
  virtual ~xsd__unsignedShort();
  xsd__unsignedShort(unsigned short uh);
  virtual void print(std::ostream &s) const;
}; 

class xsd__unsignedByte: xsd__unsignedShort
{ unsigned char __item;
  xsd__unsignedByte();
  virtual ~xsd__unsignedByte();
  xsd__unsignedByte(unsigned char uc);
  virtual void print(std::ostream &s) const;
}; 

class xsd__base64Binary: xsd__anySimpleType
{ unsigned char *__ptr;
  int __size;
  xsd__base64Binary();
  virtual ~xsd__base64Binary();
  xsd__base64Binary(char *s);
  virtual void print(std::ostream &s) const;
};

class xsd__hexBinary: xsd__anySimpleType
{ unsigned char *__ptr;
  int __size;
  xsd__hexBinary();
  virtual ~xsd__hexBinary();
  xsd__hexBinary(char *s);
  virtual void print(std::ostream &s) const;
};

class array: xsd__anyType
{ xsd__anyType **__ptr;
  int __size;
  array();
  virtual ~array();
  array(int n);
  virtual xsd__anyType*& operator[](int i);
  virtual void print(std::ostream &s) const;
};

int ns__varStringParamTest(int __size, char **param, int &return_);

int ns__varPolyParamTest(int __size, xsd__anyType **param, struct ns__varPolyParamTestResponse { int __size; xsd__anyType **param; } &out);
