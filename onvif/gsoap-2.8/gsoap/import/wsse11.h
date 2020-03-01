/*
	wsse.h 1.1

	Generated with:
	wsdl2h -cegxy -o wsse.h -t WS/WS-typemap.dat WS/wsse.xsd

	- Removed //gsoapopt
	- Added //gsoap wsse  schema import: http://docs.oasis-open.org/wss/oasis-wss-wssecurity-secext-1.1.xsd
	- Changed namespace to v1.1
	- Added SOAP_ENV__Header struct

*/

/******************************************************************************\
 *                                                                            *
 * http://docs.oasis-open.org/wss/oasis-wss-wssecurity-secext-1.1.xsd         *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Import                                                                     *
 *                                                                            *
\******************************************************************************/

#import "wsu.h"	// wsu = <http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd>

/******************************************************************************\
 *                                                                            *
 * Schema Namespaces                                                          *
 *                                                                            *
\******************************************************************************/

//gsoap wsse  schema import:		http://docs.oasis-open.org/wss/oasis-wss-wssecurity-secext-1.1.xsd
//gsoap wsse  schema namespace2:	http://docs.oasis-open.org/wss/oasis-wss-wssecurity-secext-1.0.xsd
//gsoap wsse  schema elementForm:	qualified
//gsoap wsse  schema attributeForm:	unqualified

/******************************************************************************\
 *                                                                            *
 * Schema Types                                                               *
 *                                                                            *
\******************************************************************************/



/// Imported complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.1.xsd":AttributedString from typemap WS/WS-typemap.dat.
/// @brief This type represents an element with arbitrary attributes.
/// complexType definition intentionally left blank.

/// Imported complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":PasswordString from typemap WS/WS-typemap.dat.
/// @brief This type is used for password elements per Section 4.1.
/// complexType definition intentionally left blank.

/// Imported complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":EncodedString from typemap WS/WS-typemap.dat.
/// @brief This type is used for elements containing stringified binary data.
/// complexType definition intentionally left blank.

/// Imported complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":UsernameTokenType from typemap WS/WS-typemap.dat.
/// @brief This type represents a username token per Section 4.1
/// complexType definition intentionally left blank.

/// Imported complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":BinarySecurityTokenType from typemap WS/WS-typemap.dat.
/// @brief A security token that is encoded in binary
/// complexType definition intentionally left blank.

/// Imported complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":KeyIdentifierType from typemap WS/WS-typemap.dat.
/// @brief A security token key identifier
/// complexType definition intentionally left blank.

/// Imported complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":ReferenceType from typemap WS/WS-typemap.dat.
/// @brief This type represents a reference to an external security token.
/// complexType definition intentionally left blank.

/// Imported complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":EmbeddedType from typemap WS/WS-typemap.dat.
/// @brief This type represents a reference to an embedded security token.
/// complexType definition intentionally left blank.

/// Imported complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":SecurityTokenReferenceType from typemap WS/WS-typemap.dat.
/// @brief This type is used reference a security token.
/// complexType definition intentionally left blank.

/// Imported complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":SecurityHeaderType from typemap WS/WS-typemap.dat.
/// @brief This complexType defines header block to use for security-relevant data directed at a specific SOAP actor.
/// complexType definition intentionally left blank.

/// Imported complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":TransformationParametersType from typemap WS/WS-typemap.dat.
/// @brief This complexType defines a container for elements to be specified from any namespace as properties/parameters of a DSIG transformation.
/// complexType definition intentionally left blank.

/// Imported simpleType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":tUsage from typemap WS/WS-typemap.dat.
/// @brief Typedef to allow a list of usages (as URIs).
/// simpleType definition intentionally left blank.

/// "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":FaultcodeEnum is a simpleType restriction of xs:QName.
enum wsse__FaultcodeEnum
{
	wsse__UnsupportedSecurityToken,	///< xs:QName value=""http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":UnsupportedSecurityToken"
	wsse__UnsupportedAlgorithm,	///< xs:QName value=""http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":UnsupportedAlgorithm"
	wsse__InvalidSecurity,	///< xs:QName value=""http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":InvalidSecurity"
	wsse__InvalidSecurityToken,	///< xs:QName value=""http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":InvalidSecurityToken"
	wsse__FailedAuthentication,	///< xs:QName value=""http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":FailedAuthentication"
	wsse__FailedCheck,	///< xs:QName value=""http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":FailedCheck"
	wsse__SecurityTokenUnavailable,	///< xs:QName value=""http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":SecurityTokenUnavailable"
};
/// Typedef synonym for enum wsse__FaultcodeEnum.
typedef enum wsse__FaultcodeEnum wsse__FaultcodeEnum;

/// Element "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":UsernameToken of complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":UsernameTokenType.
/// @brief This element defines the wsse:UsernameToken element per Section 4.1.
/// Imported element _wsse__UsernameToken from typemap WS/WS-typemap.dat.
typedef struct _wsse__UsernameToken
{	char*					Username;
	struct _wsse__Password*			Password;
	char*					Nonce;
	char*					wsu__Created;
	@char*					wsu__Id;
} _wsse__UsernameToken;

/// Element "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":BinarySecurityToken of complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":BinarySecurityTokenType.
/// @brief This element defines the wsse:BinarySecurityToken element per Section 4.2.
/// Imported element _wsse__BinarySecurityToken from typemap WS/WS-typemap.dat.
typedef struct _wsse__BinarySecurityToken
{	char*					__item;
	@char*					wsu__Id;
	@char*					ValueType;
	@char*					EncodingType;
} _wsse__BinarySecurityToken;

/// Element "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":Reference of complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":ReferenceType.
/// @brief This element defines a security token reference
/// Imported element _wsse__Reference from typemap WS/WS-typemap.dat.
typedef struct _wsse__Reference
{	@char*					URI;
	@char*					ValueType;
} _wsse__Reference;

/// Element "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":Embedded of complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":EmbeddedType.
/// @brief This element defines a security token embedded reference
/// Imported element _wsse__Embedded from typemap WS/WS-typemap.dat.
typedef struct _wsse__Embedded
{	/* Extensible with embedded tokens and assertions */
	@char*					wsu__Id;
	@char*					ValueType;
} _wsse__Embedded;

/// Element "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":KeyIdentifier of complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":KeyIdentifierType.
/// @brief This element defines a key identifier reference
/// Imported element _wsse__KeyIdentifier from typemap WS/WS-typemap.dat.
typedef struct _wsse__KeyIdentifier
{	char*					__item;
	@char*					wsu__Id;
	@char*					ValueType;
	@char*					EncodingType;
} _wsse__KeyIdentifier;

/// Element "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":SecurityTokenReference of complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":SecurityTokenReferenceType.
/// @brief This element defines the wsse:SecurityTokenReference per Section 4.3.
/// Imported element _wsse__SecurityTokenReference from typemap WS/WS-typemap.dat.
typedef struct _wsse__SecurityTokenReference
{	struct _wsse__Reference*		Reference;
	struct _wsse__KeyIdentifier*		KeyIdentifier;
	struct _wsse__Embedded*			Embedded;
	struct ds__X509DataType*		X509Data;
	@char*					wsu__Id;
	@char*					wsc__Instance;
	@char*					Usage;
} _wsse__SecurityTokenReference;

/// Element "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":Security of complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":SecurityHeaderType.
/// @brief This element defines the wsse:Security SOAP header element per Section 4.
/// Imported element _wsse__Security from typemap WS/WS-typemap.dat.
#import "xenc.h"
#import "wsc.h"
typedef struct _wsse__Security
{	struct _wsu__Timestamp*			wsu__Timestamp;
	struct _wsse__UsernameToken*		UsernameToken;
	struct _wsse__BinarySecurityToken*	BinarySecurityToken;
	struct xenc__EncryptedKeyType*		xenc__EncryptedKey;
	struct _xenc__ReferenceList*		xenc__ReferenceList;
	struct wsc__SecurityContextTokenType*	wsc__SecurityContextToken;
	struct ds__SignatureType*		ds__Signature;
	@char*					SOAP_ENV__actor;
	@char*					SOAP_ENV__role;
} _wsse__Security;

/// Element "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":TransformationParameters of complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":TransformationParametersType.
/// @brief This element contains properties for transformations from any namespace, including DSIG.
/// '_wsse__TransformationParameters' element definition intentionally left blank.

/// Element "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":Password of complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":PasswordString.
/// Imported element _wsse__Password from typemap WS/WS-typemap.dat.
typedef struct _wsse__Password
{	char*					__item;
	@char*					Type;
} _wsse__Password;

/// Element "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":Nonce of complexType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":EncodedString.
/// '_wsse__Nonce' element definition intentionally left blank.

/// Attribute "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":Usage of simpleType "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd":tUsage.
/// @brief This global attribute is used to indicate the usage of a referenced or indicated token within the containing context
/// '_wsse__Usage' attribute definition intentionally left blank.

struct SOAP_ENV__Header
{
  mustUnderstand _wsse__Security *wsse__Security 0;
};

/* End of wsse.h */
