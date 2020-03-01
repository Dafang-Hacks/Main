/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 51 "soapcpp2_yacc.y" /* yacc.c:339  */


#include "soapcpp2.h"

#ifdef WIN32
#ifndef __STDC__
#define __STDC__
#endif
#define YYINCLUDED_STDLIB_H
#ifdef WIN32_WITHOUT_SOLARIS_FLEX
extern int soapcpp2lex(void);
#else
extern int yylex(void);
#endif
#else
extern int yylex(void);
#endif

extern int is_XML(Tnode*);

#define MAXNEST 16	/* max. nesting depth of scopes */

struct Scope
{	Table	*table;
	Entry	*entry;
	Node	node;
	LONG64	val;
	int	offset;
	Bool	grow;	/* true if offset grows with declarations */
	Bool	mask;	/* true if enum is mask */
}	stack[MAXNEST],	/* stack of tables and offsets */
	*sp;		/* current scope stack pointer */

Table	*classtable = (Table*)0,
	*enumtable = (Table*)0,
	*typetable = (Table*)0,
	*booltable = (Table*)0,
	*templatetable = (Table*)0;

char	*namespaceid = NULL;
int	transient = 0;
int	permission = 0;
int	custom_header = 1;
int	custom_fault = 1;
Pragma	*pragmas = NULL;
Tnode	*qname = NULL;
Tnode	*xml = NULL;

/* function prototypes for support routine section */
static Entry	*undefined(Symbol*);
static Tnode	*mgtype(Tnode*, Tnode*);
static Node	op(const char*, Node, Node), iop(const char*, Node, Node), relop(const char*, Node, Node);
static void	mkscope(Table*, int), enterscope(Table*, int), exitscope(void);
static int	integer(Tnode*), real(Tnode*), numeric(Tnode*);
static void	add_soap(void), add_XML(void), add_qname(void), add_header(Table*), add_fault(Table*), add_response(Entry*, Entry*), add_result(Tnode*);
extern char	*c_storage(Storage), *c_type(Tnode*), *c_ident(Tnode*);
extern int	is_primitive_or_string(Tnode*), is_stdstr(Tnode*), is_binary(Tnode*), is_external(Tnode*), is_mutable(Tnode*), has_attachment(Tnode*);

/* Temporaries used in semantic rules */
int	i;
char	*s, *s1, *s2;
Symbol	*sym;
Entry	*p, *q;
Tnode	*t;
Node	tmp, c;
Pragma	**pp;


#line 135 "soapcpp2_yacc.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    PRAGMA = 258,
    AUTO = 259,
    DOUBLE = 260,
    INT = 261,
    STRUCT = 262,
    BREAK = 263,
    ELSE = 264,
    LONG = 265,
    SWITCH = 266,
    CASE = 267,
    ENUM = 268,
    REGISTER = 269,
    TYPEDEF = 270,
    CHAR = 271,
    EXTERN = 272,
    RETURN = 273,
    UNION = 274,
    CONST = 275,
    FLOAT = 276,
    SHORT = 277,
    UNSIGNED = 278,
    CONTINUE = 279,
    FOR = 280,
    SIGNED = 281,
    VOID = 282,
    DEFAULT = 283,
    GOTO = 284,
    SIZEOF = 285,
    VOLATILE = 286,
    DO = 287,
    IF = 288,
    STATIC = 289,
    WHILE = 290,
    CLASS = 291,
    PRIVATE = 292,
    PROTECTED = 293,
    PUBLIC = 294,
    VIRTUAL = 295,
    INLINE = 296,
    OPERATOR = 297,
    LLONG = 298,
    BOOL = 299,
    CFALSE = 300,
    CTRUE = 301,
    WCHAR = 302,
    TIME = 303,
    USING = 304,
    NAMESPACE = 305,
    ULLONG = 306,
    MUSTUNDERSTAND = 307,
    SIZE = 308,
    FRIEND = 309,
    TEMPLATE = 310,
    EXPLICIT = 311,
    TYPENAME = 312,
    RESTRICT = 313,
    null = 314,
    UCHAR = 315,
    USHORT = 316,
    UINT = 317,
    ULONG = 318,
    NONE = 319,
    ID = 320,
    LAB = 321,
    TYPE = 322,
    LNG = 323,
    DBL = 324,
    CHR = 325,
    TAG = 326,
    STR = 327,
    PA = 328,
    NA = 329,
    TA = 330,
    DA = 331,
    MA = 332,
    AA = 333,
    XA = 334,
    OA = 335,
    LA = 336,
    RA = 337,
    OR = 338,
    AN = 339,
    EQ = 340,
    NE = 341,
    LE = 342,
    GE = 343,
    LS = 344,
    RS = 345,
    AR = 346,
    PP = 347,
    NN = 348
  };
#endif
/* Tokens.  */
#define PRAGMA 258
#define AUTO 259
#define DOUBLE 260
#define INT 261
#define STRUCT 262
#define BREAK 263
#define ELSE 264
#define LONG 265
#define SWITCH 266
#define CASE 267
#define ENUM 268
#define REGISTER 269
#define TYPEDEF 270
#define CHAR 271
#define EXTERN 272
#define RETURN 273
#define UNION 274
#define CONST 275
#define FLOAT 276
#define SHORT 277
#define UNSIGNED 278
#define CONTINUE 279
#define FOR 280
#define SIGNED 281
#define VOID 282
#define DEFAULT 283
#define GOTO 284
#define SIZEOF 285
#define VOLATILE 286
#define DO 287
#define IF 288
#define STATIC 289
#define WHILE 290
#define CLASS 291
#define PRIVATE 292
#define PROTECTED 293
#define PUBLIC 294
#define VIRTUAL 295
#define INLINE 296
#define OPERATOR 297
#define LLONG 298
#define BOOL 299
#define CFALSE 300
#define CTRUE 301
#define WCHAR 302
#define TIME 303
#define USING 304
#define NAMESPACE 305
#define ULLONG 306
#define MUSTUNDERSTAND 307
#define SIZE 308
#define FRIEND 309
#define TEMPLATE 310
#define EXPLICIT 311
#define TYPENAME 312
#define RESTRICT 313
#define null 314
#define UCHAR 315
#define USHORT 316
#define UINT 317
#define ULONG 318
#define NONE 319
#define ID 320
#define LAB 321
#define TYPE 322
#define LNG 323
#define DBL 324
#define CHR 325
#define TAG 326
#define STR 327
#define PA 328
#define NA 329
#define TA 330
#define DA 331
#define MA 332
#define AA 333
#define XA 334
#define OA 335
#define LA 336
#define RA 337
#define OR 338
#define AN 339
#define EQ 340
#define NE 341
#define LE 342
#define GE 343
#define LS 344
#define RS 345
#define AR 346
#define PP 347
#define NN 348

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 125 "soapcpp2_yacc.y" /* yacc.c:355  */
	Symbol	*sym;
	LONG64	i;
	double	r;
	char	c;
	char	*s;
	Tnode	*typ;
	Storage	sto;
	Node	rec;
	Entry	*e;

#line 372 "soapcpp2_yacc.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 389 "soapcpp2_yacc.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1101

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  119
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  62
/* YYNRULES -- Number of rules.  */
#define YYNRULES  241
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  374

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   348

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   113,     2,     2,   118,   104,    91,     2,
     115,   116,   102,   100,    73,   101,     2,   103,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    86,   110,
      94,    74,    96,    85,   117,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   111,     2,   112,    90,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   108,    89,   109,   114,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    87,    88,
      92,    93,    95,    97,    98,    99,   105,   106,   107
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   199,   199,   215,   229,   231,   233,   237,   239,   240,
     241,   248,   249,   251,   274,   277,   279,   281,   283,   285,
     287,   289,   292,   295,   298,   301,   304,   305,   306,   308,
     309,   310,   311,   314,   432,   448,   449,   451,   452,   453,
     454,   455,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,   468,   469,   470,   471,   472,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   499,   509,   526,   584,   586,
     587,   589,   590,   592,   671,   678,   693,   695,   698,   702,
     715,   762,   768,   772,   782,   829,   830,   831,   832,   833,
     834,   835,   836,   837,   838,   839,   840,   841,   842,   843,
     844,   845,   846,   847,   848,   856,   878,   889,   908,   911,
     924,   946,   965,   980,   995,  1018,  1038,  1053,  1068,  1090,
    1109,  1128,  1136,  1144,  1165,  1179,  1184,  1189,  1194,  1200,
    1217,  1236,  1253,  1254,  1256,  1257,  1258,  1259,  1266,  1268,
    1275,  1283,  1289,  1290,  1292,  1300,  1301,  1302,  1303,  1304,
    1305,  1306,  1307,  1308,  1309,  1310,  1311,  1312,  1316,  1317,
    1319,  1320,  1322,  1323,  1325,  1326,  1328,  1329,  1336,  1341,
    1343,  1356,  1360,  1368,  1369,  1380,  1381,  1383,  1390,  1397,
    1404,  1411,  1419,  1420,  1422,  1423,  1424,  1433,  1434,  1437,
    1442,  1445,  1448,  1451,  1453,  1456,  1459,  1461,  1464,  1465,
    1466,  1467,  1468,  1469,  1470,  1471,  1472,  1473,  1474,  1475,
    1476,  1477,  1478,  1479,  1480,  1483,  1488,  1493,  1503,  1504,
    1511,  1515,  1520,  1523,  1524,  1531,  1535,  1539,  1543,  1547,
    1551,  1555
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PRAGMA", "AUTO", "DOUBLE", "INT",
  "STRUCT", "BREAK", "ELSE", "LONG", "SWITCH", "CASE", "ENUM", "REGISTER",
  "TYPEDEF", "CHAR", "EXTERN", "RETURN", "UNION", "CONST", "FLOAT",
  "SHORT", "UNSIGNED", "CONTINUE", "FOR", "SIGNED", "VOID", "DEFAULT",
  "GOTO", "SIZEOF", "VOLATILE", "DO", "IF", "STATIC", "WHILE", "CLASS",
  "PRIVATE", "PROTECTED", "PUBLIC", "VIRTUAL", "INLINE", "OPERATOR",
  "LLONG", "BOOL", "CFALSE", "CTRUE", "WCHAR", "TIME", "USING",
  "NAMESPACE", "ULLONG", "MUSTUNDERSTAND", "SIZE", "FRIEND", "TEMPLATE",
  "EXPLICIT", "TYPENAME", "RESTRICT", "null", "UCHAR", "USHORT", "UINT",
  "ULONG", "NONE", "ID", "LAB", "TYPE", "LNG", "DBL", "CHR", "TAG", "STR",
  "','", "'='", "PA", "NA", "TA", "DA", "MA", "AA", "XA", "OA", "LA", "RA",
  "'?'", "':'", "OR", "AN", "'|'", "'^'", "'&'", "EQ", "NE", "'<'", "LE",
  "'>'", "GE", "LS", "RS", "'+'", "'-'", "'*'", "'/'", "'%'", "AR", "PP",
  "NN", "'{'", "'}'", "';'", "'['", "']'", "'!'", "'~'", "'('", "')'",
  "'@'", "'$'", "$accept", "prog", "s1", "exts", "exts1", "ext", "pragma",
  "decls", "t1", "t2", "t3", "t4", "t5", "dclrs", "dclr", "fdclr", "id",
  "name", "constr", "destr", "func", "fname", "fargso", "fargs", "farg",
  "arg", "texp", "spec", "tspec", "type", "struct", "class", "enum",
  "tname", "base", "s2", "s3", "s4", "s5", "s6", "store", "constobj",
  "abstract", "virtual", "ptrs", "array", "arrayck", "init", "tag",
  "occurs", "patt", "cint", "expr", "cexp", "qexp", "oexp", "obex", "aexp",
  "abex", "rexp", "lexp", "pexp", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,    44,    61,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,    63,    58,   338,   339,   124,
      94,    38,   340,   341,    60,   342,    62,   343,   344,   345,
      43,    45,    42,    47,    37,   346,   347,   348,   123,   125,
      59,    91,    93,    33,   126,    40,    41,    64,    36
};
# endif

#define YYPACT_NINF -256

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-256)))

#define YYTABLE_NINF -208

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -256,    24,   -17,  -256,   -10,  -256,   213,   -70,   -60,  -256,
    -256,  -256,  -256,    16,  -256,    13,  -256,  -256,  -256,  -256,
    -256,    17,  -256,  -256,  -256,  -256,  -256,  -256,  -256,  -256,
      20,   -50,  -256,  -256,  -256,  -256,  -256,  -256,  -256,  -256,
    -256,   -31,  -256,  -256,  -256,  -256,  -256,   -24,  -256,  -256,
    -256,  -256,  -256,  -256,  -256,  -256,   -57,  -256,  -256,   -43,
     974,   -11,   -54,    11,   974,   -22,  -256,  -256,    14,    18,
      21,  -256,  -256,    33,    18,    21,    42,  -256,  -256,    45,
      18,    21,  -256,    48,    54,  -256,  -256,  -256,  -256,    36,
     974,  -256,  -256,  -256,     7,  -256,  -256,  -256,   -14,  -256,
      63,  -256,  -256,    19,  -256,  -256,  -256,    60,   676,  -256,
     443,  -256,    58,   906,  -256,   443,  -256,  -256,   443,  -256,
    -256,    42,    74,  -256,   974,   974,  -256,  -256,  -256,  -256,
     791,   -16,  -256,  -256,  -256,   443,   116,    19,    19,    19,
    -256,    79,   443,   906,  -256,  -256,    72,   102,   104,   107,
     103,   558,   -48,  -256,   148,   113,   443,   115,   129,  -256,
     -42,  -256,  -256,  -256,   974,  -256,  -256,  -256,  -256,  -256,
    -256,  -256,  -256,  -256,  -256,  -256,  -256,  -256,  -256,  -256,
    -256,  -256,  -256,  -256,  -256,  -256,  -256,  -256,  -256,  -256,
    -256,  -256,  -256,  -256,  -256,  -256,  -256,   125,  -256,  -256,
     122,  -256,    71,  -256,   170,   133,  -256,  -256,  -256,  -256,
    -256,   134,   148,  -256,  -256,  -256,  -256,  -256,   136,   328,
     906,   137,   141,  -256,   142,  -256,   209,   147,  -256,   146,
    -256,   179,  -256,  -256,  -256,   155,  -256,  -256,  -256,  -256,
    -256,  -256,  -256,  -256,   986,   986,   986,   986,   147,   986,
     986,   986,   151,    25,    64,   183,   184,   198,  -256,  -256,
    -256,   205,  -256,   443,  -256,   172,   328,   328,   328,   328,
    -256,   148,  -256,  -256,    42,  -256,   259,   974,     3,   974,
    -256,  -256,  -256,  -256,  -256,  -256,  -256,   -51,  -256,   147,
     986,   986,   986,   986,   986,   986,   986,   986,   986,   986,
     986,   986,   986,   986,   986,   986,   986,   986,   986,  -256,
     208,    34,   174,  -256,  -256,  -256,  -256,  -256,   175,  -256,
    -256,   211,  -256,  -256,   147,   187,   986,  -256,  -256,   236,
     226,   183,   198,   312,   426,   540,   656,   656,    76,    76,
      76,    76,    44,    44,    29,    29,  -256,  -256,  -256,   986,
    -256,  -256,   -27,   245,   246,   230,  -256,  -256,   249,  -256,
     205,  -256,  -256,   986,  -256,  -256,  -256,  -256,   -27,  -256,
     208,  -256,  -256,  -256
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     6,     1,     0,     2,     0,     0,     0,    13,
     155,   106,   100,     0,   101,     0,   156,   160,    97,   159,
     166,     0,   162,   105,    99,   108,   107,    95,   169,   157,
       0,   161,   164,   102,    96,    98,   113,   103,   165,   104,
     163,     0,   158,   109,   110,   111,   112,   133,    21,    22,
     167,   168,     7,     9,    11,    12,     0,    78,    78,   176,
      88,     0,   118,     0,    88,     0,     6,    10,     0,   122,
     123,   149,   139,     0,   131,   132,     0,   149,   141,     0,
     126,   127,   150,     0,     0,    35,    36,   149,   140,     0,
       0,   176,     8,    29,     0,    30,    27,    78,     0,   161,
     133,    90,   149,     0,   149,   149,    89,     0,     0,   136,
       0,   138,     0,    88,   137,     0,   150,   135,     0,   142,
     143,     0,     0,   176,    92,    91,    31,    78,   154,    28,
       0,   179,   178,   177,    34,     0,     0,     0,     0,     0,
     147,   119,     0,    88,    76,     4,     0,     0,     0,     0,
       0,     0,     0,   151,   152,     0,     0,     0,     0,   134,
     179,    94,    93,    32,    79,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    71,    69,    70,     0,    38,    39,
       0,    74,     0,   182,   185,     0,   148,   145,   144,   146,
     149,     0,   152,    20,    23,    24,    25,   120,     0,     0,
      88,   176,     0,   124,     0,   115,     0,   179,    86,     0,
      80,    81,   176,    72,    73,     0,   240,   241,   236,   234,
     235,   237,   238,   239,     0,     0,     0,     0,   179,     0,
       0,     0,     0,   200,     0,   203,     0,   206,   224,   232,
     186,   192,   121,     0,   116,     0,     0,     0,     0,     0,
      15,   152,   128,   125,     0,    87,   170,     0,    84,     0,
     230,   228,   227,   229,   181,   225,   226,     0,   198,   179,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   193,
     183,   187,     0,   129,    16,    17,    18,    19,     0,   114,
     171,   172,    82,    85,   179,     0,     0,   233,   180,   201,
       0,   202,   205,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,     0,
      33,   194,     0,     0,     0,   188,   117,   130,     0,    77,
     192,   231,   197,     0,   184,   191,   195,   196,   189,   173,
     183,   199,   190,    83
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -256,  -256,  -256,  -256,   252,  -256,  -256,  -108,     5,   101,
    -256,  -256,  -256,    -5,   261,   262,   -15,  -256,  -256,  -256,
     -38,  -256,  -256,    49,  -256,  -256,  -126,    40,  -112,   -88,
    -256,  -256,  -256,  -256,    30,   -58,   206,  -256,  -181,  -256,
     -85,  -256,  -256,  -256,  -114,  -152,     4,   -34,  -256,   -23,
    -256,  -254,  -255,  -199,  -256,  -256,  -256,    62,  -256,   -97,
      61,  -256
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     5,     6,    52,    53,   150,   151,    55,
     266,   267,   268,   152,   126,   127,    72,   134,    57,    58,
      93,    94,   229,   230,   231,   324,   122,    59,   123,    60,
      61,    62,    63,   121,   141,   110,   115,   220,   222,   164,
      64,   321,   359,    65,    98,   203,   204,   350,   261,   310,
     311,   355,   287,   288,   330,   253,   254,   255,   256,   257,
     258,   259
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      78,    56,   124,   252,   201,   125,    83,   155,   228,   160,
     157,    54,   161,   162,    73,    88,    91,    68,    79,   113,
      95,    84,   326,   227,     3,    91,   136,   205,   130,   118,
     -26,   265,   103,     4,   211,   329,   124,   124,    66,   125,
     125,   351,   124,   218,   135,   125,   142,   143,   224,   132,
      67,   131,   232,    92,   104,     7,   137,   138,   139,   129,
     133,   112,   219,    89,  -175,   327,   -26,   -26,   323,   202,
      90,   362,   119,   353,   354,   275,   124,   132,    74,   125,
      75,    69,    80,    70,    81,    85,   140,    86,   133,   163,
     318,   -75,   107,   120,   132,   202,   284,   102,   365,   -37,
     101,   235,   351,    56,   106,   133,   158,    85,   154,    86,
    -204,   270,  -204,    54,   372,    76,   236,   237,   278,   105,
     352,    77,   128,   109,    71,    82,   -35,   144,    87,   -36,
     238,   306,   307,   308,   353,   354,   239,   328,   212,   240,
     241,   242,   111,   243,   304,   305,   306,   307,   308,   290,
     364,   291,   263,   325,   114,   312,   116,    90,   314,   315,
     316,   317,   244,   117,   371,   232,   153,   207,   208,   209,
     159,   245,   246,   247,   302,   303,   304,   305,   306,   307,
     308,   206,   213,   248,   249,   250,   251,   210,   214,   124,
     215,   124,   125,   216,   125,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   217,    -5,     8,   271,     9,    10,    11,    12,
      13,   221,   223,    14,   225,   226,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,   233,   234,    26,
      27,   260,   262,   264,    28,   274,  -153,    29,    49,    30,
     272,   273,   277,    31,    32,   -88,    33,    34,   202,   319,
      35,    36,   276,   289,    37,    38,    39,    40,    41,    42,
     279,  -207,   292,    43,    44,    45,    46,   309,   -88,   320,
      47,   313,   349,   356,   357,   358,   -88,   293,   294,   295,
     296,   297,   298,   299,   300,   301,   302,   303,   304,   305,
     306,   307,   308,   361,   -88,   280,   281,   282,   283,   326,
     285,   286,   363,   366,   367,   -88,   368,   369,   108,   269,
      96,    97,   156,   -88,    48,    49,   322,  -174,   360,   146,
      50,    51,    10,    11,    12,    13,   373,   370,    14,     0,
       0,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,     0,   331,    26,    27,     0,     0,     0,    28,
       0,     0,    29,     0,    30,   147,   148,   149,    31,    32,
     -88,    33,    34,     0,     0,    35,    36,     0,     0,    37,
      38,    39,    40,    41,    42,     0,     0,     0,    43,    44,
      45,    46,     0,   -88,     0,    47,     0,     0,     0,     0,
       0,   -88,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   308,     0,     0,   -88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     -88,     0,     0,     0,     0,     0,     0,   -14,   -88,    48,
     -14,     0,  -174,     0,   146,    50,    51,    10,    11,    12,
      13,     0,     0,    14,     0,     0,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,     0,     0,    26,
      27,     0,     0,     0,    28,     0,     0,    29,     0,    30,
     147,   148,   149,    31,    32,   -88,    33,    34,     0,     0,
      35,    36,     0,     0,    37,    38,    39,    40,    41,    42,
       0,     0,     0,    43,    44,    45,    46,     0,   -88,     0,
      47,     0,     0,     0,     0,     0,   -88,   295,   296,   297,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,     0,     0,     0,   -88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   -88,     0,     0,     0,     0,
       0,     0,   -14,   -88,    48,     0,     0,  -174,     0,   146,
      50,    51,    10,    11,    12,    13,     0,     0,    14,     0,
       0,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,     0,     0,    26,    27,     0,     0,     0,    28,
       0,     0,    29,     0,    30,   147,   148,   149,    31,    32,
     -88,    33,    34,     0,     0,    35,    36,     0,     0,    37,
      38,    39,    40,    41,    42,     0,     0,     0,    43,    44,
      45,    46,     0,   -88,     0,    47,     0,     0,     0,     0,
       0,   -88,   296,   297,   298,   299,   300,   301,   302,   303,
     304,   305,   306,   307,   308,     0,     0,     0,     0,   -88,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     -88,     0,     0,     0,     0,     0,     0,     0,   -88,    48,
     -14,     0,  -174,     0,     0,    50,    51,     8,     0,     9,
      10,    11,    12,    13,     0,     0,    14,     0,     0,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
       0,     0,    26,    27,     0,     0,     0,    28,     0,     0,
      29,     0,    30,     0,     0,     0,    31,    32,   -88,    33,
      34,     0,     0,    35,    36,     0,     0,    37,    38,    39,
      40,    41,    42,     0,     0,     0,    43,    44,    45,    46,
       0,   -88,     0,    47,     0,     0,     0,     0,     0,   -88,
     298,   299,   300,   301,   302,   303,   304,   305,   306,   307,
     308,     0,     0,     0,     0,     0,     0,   -88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   -88,     0,
       0,     0,     0,     0,     0,   145,   -88,    48,    49,     0,
    -174,     0,     0,    50,    51,    10,    11,    12,    13,     0,
       0,    14,     0,     0,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,     0,     0,    26,    27,     0,
       0,     0,    28,     0,     0,    29,     0,    30,     0,     0,
       0,    99,    32,     0,    33,    34,     0,     0,    35,    36,
       0,     0,    37,    38,    39,    40,    41,    42,     0,     0,
       0,    43,    44,    45,    46,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,     0,     0,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,     0,
       0,     0,   197,     0,   198,   199,   200,     0,    50,    51,
      10,    11,    12,    13,     0,     0,    14,     0,     0,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
       0,     0,    26,    27,     0,     0,     0,    28,     0,     0,
      29,     0,    30,     0,     0,     0,    31,    32,     0,    33,
      34,     0,     0,    35,    36,     0,     0,    37,    38,    39,
      40,    41,    42,     0,     0,     0,    43,    44,    45,    46,
       0,     0,     0,    47,     0,     0,     0,     0,    10,    11,
      12,    13,     0,     0,    14,     0,     0,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,     0,     0,
      26,    27,     0,     0,     0,    28,     0,     0,    29,     0,
      30,     0,     0,     0,    99,    32,   235,    33,    34,     0,
    -174,    35,    36,    50,    51,    37,    38,    39,    40,    41,
      42,   236,   237,     0,    43,    44,    45,    46,     0,     0,
       0,   100,     0,     0,     0,   238,     0,     0,     0,     0,
       0,   239,     0,     0,   240,   241,   242,     0,   243,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   244,     0,     0,
       0,     0,     0,     0,     0,     0,   245,   246,   247,     0,
       0,    50,    51,     0,     0,     0,     0,     0,     0,   249,
     250,   251
};

static const yytype_int16 yycheck[] =
{
      15,     6,    90,   202,   130,    90,    21,   115,   160,   123,
     118,     6,   124,   125,     1,    30,    73,     1,     1,    77,
      58,     1,    73,    65,     0,    73,     7,   135,    42,    87,
      73,   212,    86,    50,   142,   290,   124,   125,   108,   124,
     125,    68,   130,   151,   102,   130,   104,   105,   156,    91,
     110,    65,   164,   110,   108,    65,    37,    38,    39,    97,
     102,    76,   110,    94,   114,   116,   109,   110,    65,   111,
      94,   326,    36,   100,   101,   227,   164,    91,    65,   164,
      67,    65,    65,    67,    67,    65,    67,    67,   102,   127,
     271,   115,   114,    57,    91,   111,   248,   108,   352,   115,
      60,    30,    68,   108,    64,   102,   121,    65,   113,    67,
      85,   219,    87,   108,   368,   102,    45,    46,   232,   108,
      86,   108,   115,   109,   108,   108,   108,    67,   108,   108,
      59,   102,   103,   104,   100,   101,    65,   289,   143,    68,
      69,    70,   109,    72,   100,   101,   102,   103,   104,    85,
     349,    87,   210,   279,   109,   263,   108,    94,   266,   267,
     268,   269,    91,   109,   363,   277,   108,   137,   138,   139,
      96,   100,   101,   102,    98,    99,   100,   101,   102,   103,
     104,    65,   110,   112,   113,   114,   115,   108,    86,   277,
      86,   279,   277,    86,   279,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   109,     0,     1,   220,     3,     4,     5,     6,
       7,    73,   109,    10,   109,    96,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,   112,   116,    26,
      27,    71,   109,   109,    31,    36,   109,    34,   112,    36,
     109,   109,    73,    40,    41,    42,    43,    44,   111,   274,
      47,    48,   116,   112,    51,    52,    53,    54,    55,    56,
     115,    88,    88,    60,    61,    62,    63,    72,    65,    20,
      67,   109,    74,   109,   109,    74,    73,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   116,    91,   244,   245,   246,   247,    73,
     249,   250,    86,    68,    68,   102,    86,    68,    66,   218,
      59,    59,   116,   110,   111,   112,   277,   114,   324,     1,
     117,   118,     4,     5,     6,     7,   370,   360,    10,    -1,
      -1,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    -1,   291,    26,    27,    -1,    -1,    -1,    31,
      -1,    -1,    34,    -1,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    -1,    47,    48,    -1,    -1,    51,
      52,    53,    54,    55,    56,    -1,    -1,    -1,    60,    61,
      62,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,
      -1,    73,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,    -1,    -1,    -1,    -1,    -1,    -1,   109,   110,   111,
     112,    -1,   114,    -1,     1,   117,   118,     4,     5,     6,
       7,    -1,    -1,    10,    -1,    -1,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    -1,    -1,    26,
      27,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    -1,    -1,
      47,    48,    -1,    -1,    51,    52,    53,    54,    55,    56,
      -1,    -1,    -1,    60,    61,    62,    63,    -1,    65,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    73,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    -1,   109,   110,   111,    -1,    -1,   114,    -1,     1,
     117,   118,     4,     5,     6,     7,    -1,    -1,    10,    -1,
      -1,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    -1,    -1,    26,    27,    -1,    -1,    -1,    31,
      -1,    -1,    34,    -1,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    -1,    -1,    47,    48,    -1,    -1,    51,
      52,    53,    54,    55,    56,    -1,    -1,    -1,    60,    61,
      62,    63,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,
      -1,    73,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,    -1,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,   111,
     112,    -1,   114,    -1,    -1,   117,   118,     1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      -1,    -1,    26,    27,    -1,    -1,    -1,    31,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    43,
      44,    -1,    -1,    47,    48,    -1,    -1,    51,    52,    53,
      54,    55,    56,    -1,    -1,    -1,    60,    61,    62,    63,
      -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    -1,    73,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,
      -1,    -1,    -1,    -1,    -1,   109,   110,   111,   112,    -1,
     114,    -1,    -1,   117,   118,     4,     5,     6,     7,    -1,
      -1,    10,    -1,    -1,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    -1,    -1,    26,    27,    -1,
      -1,    -1,    31,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    -1,    43,    44,    -1,    -1,    47,    48,
      -1,    -1,    51,    52,    53,    54,    55,    56,    -1,    -1,
      -1,    60,    61,    62,    63,    -1,    -1,    -1,    67,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    -1,    -1,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,    -1,
      -1,    -1,   111,    -1,   113,   114,   115,    -1,   117,   118,
       4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      -1,    -1,    26,    27,    -1,    -1,    -1,    31,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    -1,    43,
      44,    -1,    -1,    47,    48,    -1,    -1,    51,    52,    53,
      54,    55,    56,    -1,    -1,    -1,    60,    61,    62,    63,
      -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,     4,     5,
       6,     7,    -1,    -1,    10,    -1,    -1,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    -1,    -1,
      26,    27,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    30,    43,    44,    -1,
     114,    47,    48,   117,   118,    51,    52,    53,    54,    55,
      56,    45,    46,    -1,    60,    61,    62,    63,    -1,    -1,
      -1,    67,    -1,    -1,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    -1,    68,    69,    70,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   100,   101,   102,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,   113,
     114,   115
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   120,   121,     0,    50,   122,   123,    65,     1,     3,
       4,     5,     6,     7,    10,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    26,    27,    31,    34,
      36,    40,    41,    43,    44,    47,    48,    51,    52,    53,
      54,    55,    56,    60,    61,    62,    63,    67,   111,   112,
     117,   118,   124,   125,   127,   128,   132,   137,   138,   146,
     148,   149,   150,   151,   159,   162,   108,   110,     1,    65,
      67,   108,   135,     1,    65,    67,   102,   108,   135,     1,
      65,    67,   108,   135,     1,    65,    67,   108,   135,    94,
      94,    73,   110,   139,   140,   139,   133,   134,   163,    40,
      67,   146,   108,    86,   108,   108,   146,   114,   123,   109,
     154,   109,   135,   154,   109,   155,   108,   109,   154,    36,
      57,   152,   145,   147,   148,   159,   133,   134,   115,   139,
      42,    65,    91,   102,   136,   154,     7,    37,    38,    39,
      67,   153,   154,   154,    67,   109,     1,    37,    38,    39,
     126,   127,   132,   108,   132,   126,   155,   126,   135,    96,
     163,   147,   147,   139,   158,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   111,   113,   114,
     115,   145,   111,   164,   165,   126,    65,   153,   153,   153,
     108,   126,   132,   110,    86,    86,    86,   109,   126,   110,
     156,    73,   157,   109,   126,   109,    96,    65,   164,   141,
     142,   143,   147,   112,   116,    30,    45,    46,    59,    65,
      68,    69,    70,    72,    91,   100,   101,   102,   112,   113,
     114,   115,   172,   174,   175,   176,   177,   178,   179,   180,
      71,   167,   109,   154,   109,   157,   129,   130,   131,   128,
     126,   132,   109,   109,    36,   164,   116,    73,   163,   115,
     179,   179,   179,   179,   164,   179,   179,   171,   172,   112,
      85,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,    72,
     168,   169,   126,   109,   126,   126,   126,   126,   157,   135,
      20,   160,   142,    65,   144,   145,    73,   116,   164,   171,
     173,   176,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,    74,
     166,    68,    86,   100,   101,   170,   109,   109,    74,   161,
     165,   116,   171,    86,   172,   170,    68,    68,    86,    68,
     168,   172,   170,   166
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   119,   120,   121,   122,   122,   123,   123,   124,   124,
     124,   124,   124,   125,   126,   126,   126,   126,   126,   126,
     126,   127,   128,   129,   130,   131,   132,   132,   132,   132,
     132,   132,   132,   133,   134,   135,   135,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   137,   138,   139,   140,   141,
     141,   142,   142,   143,   144,   144,   145,   145,   146,   146,
     146,   147,   147,   147,   147,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   149,
     150,   151,   152,   152,   153,   153,   153,   153,   153,   154,
     155,   156,   157,   157,   158,   159,   159,   159,   159,   159,
     159,   159,   159,   159,   159,   159,   159,   159,   159,   159,
     160,   160,   161,   161,   162,   162,   163,   163,   163,   164,
     164,   164,   165,   166,   166,   167,   167,   168,   168,   168,
     168,   168,   169,   169,   170,   170,   170,   171,   171,   172,
     172,   173,   174,   174,   175,   176,   176,   177,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   179,   179,   179,   179,   179,
     179,   179,   179,   180,   180,   180,   180,   180,   180,   180,
     180,   180
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     0,     5,     1,     0,     2,     2,     1,
       2,     1,     1,     1,     0,     3,     4,     4,     4,     4,
       2,     1,     1,     0,     0,     0,     1,     2,     3,     2,
       2,     3,     4,     6,     2,     1,     1,     1,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     3,     3,     2,     1,     3,     7,     0,     0,
       1,     1,     3,     6,     0,     1,     3,     4,     0,     2,
       2,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     7,     5,     5,     7,     1,     3,
       5,     5,     2,     2,     5,     6,     2,     2,     6,     6,
       8,     2,     2,     1,     4,     3,     3,     3,     3,     2,
       2,     2,     1,     1,     2,     2,     2,     1,     2,     0,
       0,     0,     0,     1,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     0,     2,     0,     1,     0,     2,     2,     0,
       4,     3,     1,     0,     2,     0,     1,     1,     2,     3,
       4,     3,     0,     1,     1,     2,     2,     3,     1,     5,
       1,     1,     3,     1,     1,     3,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     2,     2,     2,     2,     2,
       2,     4,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 199 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if (lflag)
    			  {	custom_header = 0;
    			  	custom_fault = 0;
			  }
			  else
			  {	add_header(sp->table);
			  	add_fault(sp->table);
			  }
			  compile(sp->table);
			  freetable(classtable);
			  freetable(enumtable);
			  freetable(typetable);
			  freetable(booltable);
			  freetable(templatetable);
			}
#line 1923 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 3:
#line 215 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { classtable = mktable((Table*)0);
			  enumtable = mktable((Table*)0);
			  typetable = mktable((Table*)0);
			  booltable = mktable((Table*)0);
			  templatetable = mktable((Table*)0);
			  p = enter(booltable, lookup("false"));
			  p->info.typ = mkint();
			  p->info.val.i = 0;
			  p = enter(booltable, lookup("true"));
			  p->info.typ = mkint();
			  p->info.val.i = 1;
			  mkscope(mktable(mktable((Table*)0)), 0);
			}
#line 1941 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 4:
#line 230 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { namespaceid = (yyvsp[-3].sym)->name; }
#line 1947 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 5:
#line 231 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 1953 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 6:
#line 233 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { add_soap();
			  add_qname();
			  add_XML();
			}
#line 1962 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 7:
#line 237 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 1968 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 8:
#line 239 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 1974 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 9:
#line 240 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 1980 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 10:
#line 241 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { synerror("input before ; skipped");
			  while (sp > stack)
			  {	freetable(sp->table);
			  	exitscope();
			  }
			  yyerrok;
			}
#line 1992 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 11:
#line 248 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 1998 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 12:
#line 249 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 2004 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 13:
#line 251 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((yyvsp[0].s)[1] >= 'a' && (yyvsp[0].s)[1] <= 'z')
			  {	for (pp = &pragmas; *pp; pp = &(*pp)->next)
			          ;
				*pp = (Pragma*)emalloc(sizeof(Pragma));
				(*pp)->pragma = (char*)emalloc(strlen((yyvsp[0].s))+1);
				strcpy((*pp)->pragma, (yyvsp[0].s));
				(*pp)->next = NULL;
			  }
			  else if ((i = atoi((yyvsp[0].s)+2)) > 0)
				yylineno = i;
			  else
			  {	sprintf(errbuf, "directive '%s' ignored (use #import to import files)", (yyvsp[0].s));
			  	semwarn(errbuf);
			  }
			}
#line 2024 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 14:
#line 274 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { transient &= ~6;
			  permission = 0;
			}
#line 2032 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 15:
#line 278 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 2038 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 16:
#line 280 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 2044 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 17:
#line 282 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 2050 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 18:
#line 284 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 2056 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 19:
#line 286 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 2062 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 20:
#line 287 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { synerror("declaration expected"); yyerrok; }
#line 2068 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 21:
#line 289 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { transient |= 1;
			}
#line 2075 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 22:
#line 292 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { transient &= ~1;
			}
#line 2082 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 23:
#line 295 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { permission = Sprivate;
			}
#line 2089 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 24:
#line 298 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { permission = Sprotected;
			}
#line 2096 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 25:
#line 301 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { permission = 0;
			}
#line 2103 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 26:
#line 304 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 2109 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 27:
#line 305 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 2115 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 28:
#line 307 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 2121 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 29:
#line 308 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 2127 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 30:
#line 309 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 2133 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 31:
#line 310 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 2139 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 32:
#line 312 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 2145 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 33:
#line 315 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if (((yyvsp[-3].rec).sto & Stypedef) && sp->table->level == GLOBAL)
			  {	if (((yyvsp[-3].rec).typ->type != Tstruct && (yyvsp[-3].rec).typ->type != Tunion && (yyvsp[-3].rec).typ->type != Tenum) || strcmp((yyvsp[-4].sym)->name, (yyvsp[-3].rec).typ->id->name))
				{	p = enter(typetable, (yyvsp[-4].sym));
					p->info.typ = mksymtype((yyvsp[-3].rec).typ, (yyvsp[-4].sym));
			  		if ((yyvsp[-3].rec).sto & Sextern)
						p->info.typ->transient = -1;
					else
						p->info.typ->transient = (yyvsp[-3].rec).typ->transient;
			  		p->info.sto = (yyvsp[-3].rec).sto;
					p->info.typ->pattern = (yyvsp[-1].rec).pattern;
					p->info.typ->minLength = (yyvsp[-1].rec).minLength;
					p->info.typ->maxLength = (yyvsp[-1].rec).maxLength;
				}
				(yyvsp[-4].sym)->token = TYPE;
			  }
			  else
			  {	p = enter(sp->table, (yyvsp[-4].sym));
			  	p->tag = (yyvsp[-2].s);
			  	p->info.typ = (yyvsp[-3].rec).typ;
			  	p->info.sto = (Storage)((int)(yyvsp[-3].rec).sto | permission);
				if ((yyvsp[0].rec).hasval)
				{	p->info.hasval = True;
					switch ((yyvsp[-3].rec).typ->type)
					{	case Tchar:
						case Tuchar:
						case Tshort:
						case Tushort:
						case Tint:
						case Tuint:
						case Tlong:
						case Tulong:
						case Tllong:
						case Tullong:
						case Tenum:
						case Ttime:
							if ((yyvsp[0].rec).typ->type == Tint || (yyvsp[0].rec).typ->type == Tchar || (yyvsp[0].rec).typ->type == Tenum)
								sp->val = p->info.val.i = (yyvsp[0].rec).val.i;
							else
							{	semerror("type error in initialization constant");
								p->info.hasval = False;
							}
							break;
						case Tfloat:
						case Tdouble:
						case Tldouble:
							if ((yyvsp[0].rec).typ->type == Tfloat || (yyvsp[0].rec).typ->type == Tdouble || (yyvsp[0].rec).typ->type == Tldouble)
								p->info.val.r = (yyvsp[0].rec).val.r;
							else if ((yyvsp[0].rec).typ->type == Tint)
								p->info.val.r = (double)(yyvsp[0].rec).val.i;
							else
							{	semerror("type error in initialization constant");
								p->info.hasval = False;
							}
							break;
						default:
							if ((yyvsp[-3].rec).typ->type == Tpointer
							 && (((Tnode*)(yyvsp[-3].rec).typ->ref)->type == Tchar || ((Tnode*)(yyvsp[-3].rec).typ->ref)->type == Twchar)
							 && (yyvsp[0].rec).typ->type == Tpointer
							 && ((Tnode*)(yyvsp[0].rec).typ->ref)->type == Tchar)
								p->info.val.s = (yyvsp[0].rec).val.s;
							else if (bflag
							 && (yyvsp[-3].rec).typ->type == Tarray
							 && ((Tnode*)(yyvsp[-3].rec).typ->ref)->type == Tchar
							 && (yyvsp[0].rec).typ->type == Tpointer
							 && ((Tnode*)(yyvsp[0].rec).typ->ref)->type == Tchar)
							{	if ((yyvsp[-3].rec).typ->width / ((Tnode*)(yyvsp[-3].rec).typ->ref)->width - 1 < strlen((yyvsp[0].rec).val.s))
								{	semerror("char[] initialization constant too long");
									p->info.val.s = "";
								}

								else
									p->info.val.s = (yyvsp[0].rec).val.s;
							}
							else if ((yyvsp[-3].rec).typ->type == Tpointer
							      && (((Tnode*)(yyvsp[-3].rec).typ->ref)->id == lookup("std::string") || ((Tnode*)(yyvsp[-3].rec).typ->ref)->id == lookup("std::wstring")))
							      	p->info.val.s = (yyvsp[0].rec).val.s;
							else if ((yyvsp[-3].rec).typ->id == lookup("std::string") || (yyvsp[-3].rec).typ->id == lookup("std::wstring"))
							      	p->info.val.s = (yyvsp[0].rec).val.s;
							else if ((yyvsp[-3].rec).typ->type == Tpointer
							      && (yyvsp[0].rec).typ->type == Tint
							      && (yyvsp[0].rec).val.i == 0)
								p->info.val.i = 0;
							else
							{	semerror("type error in initialization constant");
								p->info.hasval = False;
							}
							break;
					}
				}
				else
					p->info.val.i = sp->val;
			        if ((yyvsp[-1].rec).minOccurs < 0)
			        {	if ((yyvsp[0].rec).hasval || ((yyvsp[-3].rec).sto & Sattribute) || (yyvsp[-3].rec).typ->type == Tpointer || (yyvsp[-3].rec).typ->type == Ttemplate || !strncmp((yyvsp[-4].sym)->name, "__size", 6))
			        		p->info.minOccurs = 0;
			        	else
			        		p->info.minOccurs = 1;
				}
				else
					p->info.minOccurs = (yyvsp[-1].rec).minOccurs;
				p->info.maxOccurs = (yyvsp[-1].rec).maxOccurs;
				if (sp->mask)
					sp->val <<= 1;
				else
					sp->val++;
			  	p->info.offset = sp->offset;
				if ((yyvsp[-3].rec).sto & Sextern)
					p->level = GLOBAL;
				else if ((yyvsp[-3].rec).sto & Stypedef)
					;
			  	else if (sp->grow)
					sp->offset += p->info.typ->width;
				else if (p->info.typ->width > sp->offset)
					sp->offset = p->info.typ->width;
			  }
			  sp->entry = p;
			}
#line 2266 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 34:
#line 432 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((yyvsp[-1].rec).sto & Stypedef)
			  {	sprintf(errbuf, "invalid typedef qualifier for '%s'", (yyvsp[0].sym)->name);
				semwarn(errbuf);
			  }
			  p = enter(sp->table, (yyvsp[0].sym));
			  p->info.typ = (yyvsp[-1].rec).typ;
			  p->info.sto = (yyvsp[-1].rec).sto;
			  p->info.hasval = False;
			  p->info.offset = sp->offset;
			  if (sp->grow)
				sp->offset += p->info.typ->width;
			  else if (p->info.typ->width > sp->offset)
				sp->offset = p->info.typ->width;
			  sp->entry = p;
			}
#line 2286 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 35:
#line 448 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = (yyvsp[0].sym); }
#line 2292 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 36:
#line 449 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = (yyvsp[0].sym); }
#line 2298 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 37:
#line 451 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = (yyvsp[0].sym); }
#line 2304 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 38:
#line 452 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator!"); }
#line 2310 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 39:
#line 453 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator~"); }
#line 2316 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 40:
#line 454 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator="); }
#line 2322 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 41:
#line 455 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator+="); }
#line 2328 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 42:
#line 456 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator-="); }
#line 2334 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 43:
#line 457 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator*="); }
#line 2340 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 44:
#line 458 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator/="); }
#line 2346 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 45:
#line 459 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator%="); }
#line 2352 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 46:
#line 460 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator&="); }
#line 2358 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 47:
#line 461 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator^="); }
#line 2364 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 48:
#line 462 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator|="); }
#line 2370 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 49:
#line 463 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator<<="); }
#line 2376 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 50:
#line 464 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator>>="); }
#line 2382 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 51:
#line 465 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator||"); }
#line 2388 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 52:
#line 466 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator&&"); }
#line 2394 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 53:
#line 467 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator|"); }
#line 2400 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 54:
#line 468 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator^"); }
#line 2406 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 55:
#line 469 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator&"); }
#line 2412 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 56:
#line 470 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator=="); }
#line 2418 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 57:
#line 471 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator!="); }
#line 2424 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 58:
#line 472 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator<"); }
#line 2430 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 59:
#line 473 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator<="); }
#line 2436 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 60:
#line 474 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator>"); }
#line 2442 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 61:
#line 475 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator>="); }
#line 2448 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 62:
#line 476 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator<<"); }
#line 2454 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 63:
#line 477 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator>>"); }
#line 2460 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 64:
#line 478 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator+"); }
#line 2466 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 65:
#line 479 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator-"); }
#line 2472 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 66:
#line 480 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator*"); }
#line 2478 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 67:
#line 481 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator/"); }
#line 2484 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 68:
#line 482 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator%"); }
#line 2490 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 69:
#line 483 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator++"); }
#line 2496 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 70:
#line 484 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator--"); }
#line 2502 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 71:
#line 485 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator->"); }
#line 2508 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 72:
#line 486 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator[]"); }
#line 2514 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 73:
#line 487 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sym) = lookup("operator()"); }
#line 2520 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 74:
#line 488 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { s1 = c_storage((yyvsp[0].rec).sto);
			  s2 = c_type((yyvsp[0].rec).typ);
			  s = (char*)emalloc(strlen(s1) + strlen(s2) + 10);
			  strcpy(s, "operator ");
			  strcat(s, s1);
			  strcat(s, s2);
			  (yyval.sym) = lookup(s);
			  if (!(yyval.sym))
				(yyval.sym) = install(s, ID);
			}
#line 2535 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 75:
#line 499 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if (!(p = entry(classtable, (yyvsp[0].sym))))
			  	semerror("invalid constructor");
			  sp->entry = enter(sp->table, (yyvsp[0].sym));
			  sp->entry->info.typ = mknone();
			  sp->entry->info.sto = Snone;
			  sp->entry->info.offset = sp->offset;
			  sp->node.typ = mkvoid();
			  sp->node.sto = Snone;
			}
#line 2549 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 76:
#line 510 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if (!(p = entry(classtable, (yyvsp[0].sym))))
			  	semerror("invalid destructor");
			  s = (char*)emalloc(strlen((yyvsp[0].sym)->name) + 2);
			  strcpy(s, "~");
			  strcat(s, (yyvsp[0].sym)->name);
			  sym = lookup(s);
			  if (!sym)
				sym = install(s, ID);
			  sp->entry = enter(sp->table, sym);
			  sp->entry->info.typ = mknone();
			  sp->entry->info.sto = (yyvsp[-2].sto);
			  sp->entry->info.offset = sp->offset;
			  sp->node.typ = mkvoid();
			  sp->node.sto = Snone;
			}
#line 2569 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 77:
#line 527 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((yyvsp[-6].e)->level == GLOBAL)
			  {	if (!((yyvsp[-6].e)->info.sto & Sextern) && sp->entry && sp->entry->info.typ->type == Tpointer && ((Tnode*)sp->entry->info.typ->ref)->type == Tchar)
			  	{	sprintf(errbuf, "last output parameter of remote method function prototype '%s' is a pointer to a char which will only return one byte: use char** instead to return a string", (yyvsp[-6].e)->sym->name);
					semwarn(errbuf);
				}
				if ((yyvsp[-6].e)->info.sto & Sextern)
				 	(yyvsp[-6].e)->info.typ = mkmethod((yyvsp[-6].e)->info.typ, sp->table);
			  	else if (sp->entry && (sp->entry->info.typ->type == Tpointer || sp->entry->info.typ->type == Treference || sp->entry->info.typ->type == Tarray || is_transient(sp->entry->info.typ)))
				{	if ((yyvsp[-6].e)->info.typ->type == Tint)
					{	sp->entry->info.sto = (Storage)((int)sp->entry->info.sto | (int)Sreturn);
						(yyvsp[-6].e)->info.typ = mkfun(sp->entry);
						(yyvsp[-6].e)->info.typ->id = (yyvsp[-6].e)->sym;
						if (!is_transient(sp->entry->info.typ))
						{	if (!is_response(sp->entry->info.typ))
							{	if (!is_XML(sp->entry->info.typ))
									add_response((yyvsp[-6].e), sp->entry);
							}
							else
								add_result(sp->entry->info.typ);
						}
					}
					else
					{	sprintf(errbuf, "return type of remote method function prototype '%s' must be integer", (yyvsp[-6].e)->sym->name);
						semerror(errbuf);
					}
				}
			  	else
			  	{	sprintf(errbuf, "last output parameter of remote method function prototype '%s' is a return parameter and must be a pointer or reference, or use %s(..., void) for one-way sends", (yyvsp[-6].e)->sym->name, (yyvsp[-6].e)->sym->name);
					semerror(errbuf);
			  	}
				if (!((yyvsp[-6].e)->info.sto & Sextern))
			  	{	unlinklast(sp->table);
			  		if ((p = entry(classtable, (yyvsp[-6].e)->sym)))
					{	if (p->info.typ->ref)
						{	sprintf(errbuf, "remote method name clash: struct/class '%s' already declared at line %d", (yyvsp[-6].e)->sym->name, p->lineno);
							semerror(errbuf);
						}
						else
						{	p->info.typ->ref = sp->table;
							p->info.typ->width = sp->offset;
						}
					}
			  		else
			  		{	p = enter(classtable, (yyvsp[-6].e)->sym);
						p->info.typ = mkstruct(sp->table, sp->offset);
						p->info.typ->id = (yyvsp[-6].e)->sym;
			  		}
			  	}
			  }
			  else if ((yyvsp[-6].e)->level == INTERNAL)
			  {	(yyvsp[-6].e)->info.typ = mkmethod((yyvsp[-6].e)->info.typ, sp->table);
				(yyvsp[-6].e)->info.sto = (Storage)((int)(yyvsp[-6].e)->info.sto | (int)(yyvsp[-1].sto) | (int)(yyvsp[0].sto));
			  	transient &= ~1;
			  }
			  exitscope();
			}
#line 2630 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 78:
#line 584 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.e) = sp->entry; }
#line 2636 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 79:
#line 586 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 2642 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 80:
#line 587 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 2648 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 81:
#line 589 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 2654 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 82:
#line 590 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 2660 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 83:
#line 593 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((yyvsp[-2].rec).sto & Stypedef)
			  	semwarn("typedef in function argument");
			  p = enter(sp->table, (yyvsp[-3].sym));
			  p->info.typ = (yyvsp[-2].rec).typ;
			  p->info.sto = (yyvsp[-2].rec).sto;
			  if ((yyvsp[-1].rec).minOccurs < 0)
			  {	if ((yyvsp[0].rec).hasval || ((yyvsp[-2].rec).sto & Sattribute) || (yyvsp[-2].rec).typ->type == Tpointer)
			        	p->info.minOccurs = 0;
			       	else
			        	p->info.minOccurs = 1;
			  }
			  else
				p->info.minOccurs = (yyvsp[-1].rec).minOccurs;
			  p->info.maxOccurs = (yyvsp[-1].rec).maxOccurs;
			  if ((yyvsp[0].rec).hasval)
			  {	p->info.hasval = True;
				switch ((yyvsp[-2].rec).typ->type)
				{	case Tchar:
					case Tuchar:
					case Tshort:
					case Tushort:
					case Tint:
					case Tuint:
					case Tlong:
					case Tulong:
					case Tenum:
					case Ttime:
						if ((yyvsp[0].rec).typ->type == Tint || (yyvsp[0].rec).typ->type == Tchar || (yyvsp[0].rec).typ->type == Tenum)
							sp->val = p->info.val.i = (yyvsp[0].rec).val.i;
						else
						{	semerror("type error in initialization constant");
							p->info.hasval = False;
						}
						break;
					case Tfloat:
					case Tdouble:
					case Tldouble:
						if ((yyvsp[0].rec).typ->type == Tfloat || (yyvsp[0].rec).typ->type == Tdouble || (yyvsp[0].rec).typ->type == Tldouble)
							p->info.val.r = (yyvsp[0].rec).val.r;
						else if ((yyvsp[0].rec).typ->type == Tint)
							p->info.val.r = (double)(yyvsp[0].rec).val.i;
						else
						{	semerror("type error in initialization constant");
							p->info.hasval = False;
						}
						break;
					default:
						if ((yyvsp[-2].rec).typ->type == Tpointer
						 && (((Tnode*)(yyvsp[-2].rec).typ->ref)->type == Tchar || ((Tnode*)(yyvsp[-2].rec).typ->ref)->type == Twchar)
						 && (yyvsp[0].rec).typ->type == Tpointer
						 && ((Tnode*)(yyvsp[0].rec).typ->ref)->type == Tchar)
							p->info.val.s = (yyvsp[0].rec).val.s;
						else if ((yyvsp[-2].rec).typ->type == Tpointer
						      && (((Tnode*)(yyvsp[-2].rec).typ->ref)->id == lookup("std::string") || ((Tnode*)(yyvsp[-2].rec).typ->ref)->id == lookup("std::wstring")))
						      	p->info.val.s = (yyvsp[0].rec).val.s;
						else if ((yyvsp[-2].rec).typ->id == lookup("std::string") || (yyvsp[-2].rec).typ->id == lookup("std::wstring"))
						      	p->info.val.s = (yyvsp[0].rec).val.s;
						else if ((yyvsp[-2].rec).typ->type == Tpointer
						      && (yyvsp[0].rec).typ->type == Tint
						      && (yyvsp[0].rec).val.i == 0)
							p->info.val.i = 0;
						else
						{	semerror("type error in initialization constant");
							p->info.hasval = False;
						}
						break;
				}
			  }
			  p->info.offset = sp->offset;
			  if ((yyvsp[-2].rec).sto & Sextern)
				p->level = GLOBAL;
			  else if (sp->grow)
				sp->offset += p->info.typ->width;
			  else if (p->info.typ->width > sp->offset)
				sp->offset = p->info.typ->width;
			  sp->entry = p;
			}
#line 2742 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 84:
#line 671 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if (sp->table->level != PARAM)
			    (yyval.sym) = gensymidx("param", (int)++sp->val);
			  else if (eflag)
				(yyval.sym) = gensymidx("_param", (int)++sp->val);
			  else
				(yyval.sym) = gensym("_param");
			}
#line 2754 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 85:
#line 678 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if (vflag == 2 && *(yyvsp[0].sym)->name == '_' && sp->table->level == GLOBAL)
			  { sprintf(errbuf, "SOAP 1.2 does not support anonymous parameters '%s'", (yyvsp[0].sym)->name);
			    semwarn(errbuf);
			  }
			  (yyval.sym) = (yyvsp[0].sym);
			}
#line 2765 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 86:
#line 694 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = (yyvsp[0].rec); }
#line 2771 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 87:
#line 696 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = (yyvsp[0].rec); }
#line 2777 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 88:
#line 698 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).typ = mkint();
			  (yyval.rec).sto = Snone;
			  sp->node = (yyval.rec);
			}
#line 2786 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 89:
#line 702 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if (((yyvsp[-1].sto) & Stypedef) && is_external((yyvsp[0].rec).typ) && (yyvsp[0].rec).typ->type != Tstruct && (yyvsp[0].rec).typ->type != Tclass)
			  	(yyval.rec).typ = mktype((yyvsp[0].rec).typ->type, (yyvsp[0].rec).typ->ref, (yyvsp[0].rec).typ->width);
			  else
			  	(yyval.rec).typ = (yyvsp[0].rec).typ;
			  (yyval.rec).sto = (Storage)((int)(yyvsp[-1].sto) | ((int)((yyvsp[0].rec).sto)));
			  if (((yyval.rec).sto & Sattribute) && !is_primitive_or_string((yyvsp[0].rec).typ) && !is_stdstr((yyvsp[0].rec).typ) && !is_binary((yyvsp[0].rec).typ) && !is_external((yyvsp[0].rec).typ))
			  {	semwarn("invalid attribute type");
			  	(yyval.rec).sto = (Storage)((int)(yyval.rec).sto & ~Sattribute);
			  }
			  sp->node = (yyval.rec);
			  if ((yyvsp[-1].sto) & Sextern)
				transient = 0;
			}
#line 2804 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 90:
#line 715 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((yyvsp[-1].typ)->type == Tint)
				switch ((yyvsp[0].rec).typ->type)
				{ case Tchar:	(yyval.rec).typ = (yyvsp[0].rec).typ; break;
				  case Tshort:	(yyval.rec).typ = (yyvsp[0].rec).typ; break;
				  case Tint:	(yyval.rec).typ = (yyvsp[-1].typ); break;
				  case Tlong:	(yyval.rec).typ = (yyvsp[0].rec).typ; break;
				  case Tllong:	(yyval.rec).typ = (yyvsp[0].rec).typ; break;
				  default:	semwarn("illegal use of 'signed'");
						(yyval.rec).typ = (yyvsp[0].rec).typ;
				}
			  else if ((yyvsp[-1].typ)->type == Tuint)
				switch ((yyvsp[0].rec).typ->type)
				{ case Tchar:	(yyval.rec).typ = mkuchar(); break;
				  case Tshort:	(yyval.rec).typ = mkushort(); break;
				  case Tint:	(yyval.rec).typ = (yyvsp[-1].typ); break;
				  case Tlong:	(yyval.rec).typ = mkulong(); break;
				  case Tllong:	(yyval.rec).typ = mkullong(); break;
				  default:	semwarn("illegal use of 'unsigned'");
						(yyval.rec).typ = (yyvsp[0].rec).typ;
				}
			  else if ((yyvsp[-1].typ)->type == Tlong)
				switch ((yyvsp[0].rec).typ->type)
				{ case Tint:	(yyval.rec).typ = (yyvsp[-1].typ); break;
				  case Tlong:	(yyval.rec).typ = mkllong(); break;
				  case Tuint:	(yyval.rec).typ = mkulong(); break;
				  case Tulong:	(yyval.rec).typ = mkullong(); break;
				  case Tdouble:	(yyval.rec).typ = mkldouble(); break;
				  default:	semwarn("illegal use of 'long'");
						(yyval.rec).typ = (yyvsp[0].rec).typ;
				}
			  else if ((yyvsp[-1].typ)->type == Tulong)
				switch ((yyvsp[0].rec).typ->type)
				{ case Tint:	(yyval.rec).typ = (yyvsp[-1].typ); break;
				  case Tlong:	(yyval.rec).typ = mkullong(); break;
				  case Tuint:	(yyval.rec).typ = (yyvsp[-1].typ); break;
				  case Tulong:	(yyval.rec).typ = mkullong(); break;
				  default:	semwarn("illegal use of 'long'");
						(yyval.rec).typ = (yyvsp[0].rec).typ;
				}
			  else if ((yyvsp[0].rec).typ->type == Tint)
				(yyval.rec).typ = (yyvsp[-1].typ);
			  else
			  	semwarn("invalid type (missing ';' or type name used as non-type identifier?)");
			  (yyval.rec).sto = (yyvsp[0].rec).sto;
			  sp->node = (yyval.rec);
			}
#line 2855 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 91:
#line 762 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).typ = mkint();
			  (yyval.rec).sto = (yyvsp[0].sto);
			  sp->node = (yyval.rec);
			  if ((yyvsp[0].sto) & Sextern)
				transient = 0;
			}
#line 2866 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 92:
#line 768 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).typ = (yyvsp[0].typ);
			  (yyval.rec).sto = Snone;
			  sp->node = (yyval.rec);
			}
#line 2875 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 93:
#line 772 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).typ = (yyvsp[0].rec).typ;
			  (yyval.rec).sto = (Storage)((int)(yyvsp[-1].sto) | (int)(yyvsp[0].rec).sto);
			  if (((yyval.rec).sto & Sattribute) && !is_primitive_or_string((yyvsp[0].rec).typ) && !is_stdstr((yyvsp[0].rec).typ) && !is_binary((yyvsp[0].rec).typ) && !is_external((yyvsp[0].rec).typ))
			  {	semwarn("invalid attribute type");
			  	(yyval.rec).sto = (Storage)((int)(yyval.rec).sto & ~Sattribute);
			  }
			  sp->node = (yyval.rec);
			  if ((yyvsp[-1].sto) & Sextern)
				transient = 0;
			}
#line 2890 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 94:
#line 782 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((yyvsp[-1].typ)->type == Tint)
				switch ((yyvsp[0].rec).typ->type)
				{ case Tchar:	(yyval.rec).typ = (yyvsp[0].rec).typ; break;
				  case Tshort:	(yyval.rec).typ = (yyvsp[0].rec).typ; break;
				  case Tint:	(yyval.rec).typ = (yyvsp[-1].typ); break;
				  case Tlong:	(yyval.rec).typ = (yyvsp[0].rec).typ; break;
				  case Tllong:	(yyval.rec).typ = (yyvsp[0].rec).typ; break;
				  default:	semwarn("illegal use of 'signed'");
						(yyval.rec).typ = (yyvsp[0].rec).typ;
				}
			  else if ((yyvsp[-1].typ)->type == Tuint)
				switch ((yyvsp[0].rec).typ->type)
				{ case Tchar:	(yyval.rec).typ = mkuchar(); break;
				  case Tshort:	(yyval.rec).typ = mkushort(); break;
				  case Tint:	(yyval.rec).typ = (yyvsp[-1].typ); break;
				  case Tlong:	(yyval.rec).typ = mkulong(); break;
				  case Tllong:	(yyval.rec).typ = mkullong(); break;
				  default:	semwarn("illegal use of 'unsigned'");
						(yyval.rec).typ = (yyvsp[0].rec).typ;
				}
			  else if ((yyvsp[-1].typ)->type == Tlong)
				switch ((yyvsp[0].rec).typ->type)
				{ case Tint:	(yyval.rec).typ = (yyvsp[-1].typ); break;
				  case Tlong:	(yyval.rec).typ = mkllong(); break;
				  case Tuint:	(yyval.rec).typ = mkulong(); break;
				  case Tulong:	(yyval.rec).typ = mkullong(); break;
				  case Tdouble:	(yyval.rec).typ = mkldouble(); break;
				  default:	semwarn("illegal use of 'long'");
						(yyval.rec).typ = (yyvsp[0].rec).typ;
				}
			  else if ((yyvsp[-1].typ)->type == Tulong)
				switch ((yyvsp[0].rec).typ->type)
				{ case Tint:	(yyval.rec).typ = (yyvsp[-1].typ); break;
				  case Tlong:	(yyval.rec).typ = mkullong(); break;
				  case Tuint:	(yyval.rec).typ = (yyvsp[-1].typ); break;
				  case Tulong:	(yyval.rec).typ = mkullong(); break;
				  default:	semwarn("illegal use of 'long'");
						(yyval.rec).typ = (yyvsp[0].rec).typ;
				}
			  else if ((yyvsp[0].rec).typ->type == Tint)
				(yyval.rec).typ = (yyvsp[-1].typ);
			  else
			  	semwarn("invalid type");
			  (yyval.rec).sto = (yyvsp[0].rec).sto;
			  sp->node = (yyval.rec);
			}
#line 2941 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 95:
#line 829 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.typ) = mkvoid(); }
#line 2947 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 96:
#line 830 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.typ) = mkbool(); }
#line 2953 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 97:
#line 831 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.typ) = mkchar(); }
#line 2959 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 98:
#line 832 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.typ) = mkwchart(); }
#line 2965 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 99:
#line 833 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.typ) = mkshort(); }
#line 2971 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 100:
#line 834 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.typ) = mkint(); }
#line 2977 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 101:
#line 835 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.typ) = mklong(); }
#line 2983 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 102:
#line 836 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.typ) = mkllong(); }
#line 2989 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 103:
#line 837 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.typ) = mkullong(); }
#line 2995 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 104:
#line 838 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.typ) = mkulong(); }
#line 3001 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 105:
#line 839 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.typ) = mkfloat(); }
#line 3007 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 106:
#line 840 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.typ) = mkdouble(); }
#line 3013 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 107:
#line 841 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.typ) = mkint(); }
#line 3019 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 108:
#line 842 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.typ) = mkuint(); }
#line 3025 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 109:
#line 843 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.typ) = mkuchar(); }
#line 3031 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 110:
#line 844 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.typ) = mkushort(); }
#line 3037 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 111:
#line 845 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.typ) = mkuint(); }
#line 3043 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 112:
#line 846 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.typ) = mkulong(); }
#line 3049 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 113:
#line 847 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.typ) = mktimet(); }
#line 3055 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 114:
#line 849 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if (!(p = entry(templatetable, (yyvsp[0].sym))))
			  {	p = enter(templatetable, (yyvsp[0].sym));
			  	p->info.typ = mktemplate(NULL, (yyvsp[0].sym));
			  	(yyvsp[0].sym)->token = TYPE;
			  }
			  (yyval.typ) = p->info.typ;
			}
#line 3067 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 115:
#line 857 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { sym = gensym("_Struct");
			  sprintf(errbuf, "anonymous class will be named '%s'", sym->name);
			  semwarn(errbuf);
			  if ((p = entry(classtable, sym)))
			  {	if (p->info.typ->ref || p->info.typ->type != Tclass)
				{	sprintf(errbuf, "class '%s' already declared at line %d", sym->name, p->lineno);
					semerror(errbuf);
				}
			  }
			  else
			  {	p = enter(classtable, sym);
				p->info.typ = mkclass((Table*)0, 0);
			  }
			  sym->token = TYPE;
			  sp->table->sym = sym;
			  p->info.typ->ref = sp->table;
			  p->info.typ->width = sp->offset;
			  p->info.typ->id = sym;
			  (yyval.typ) = p->info.typ;
			  exitscope();
			}
#line 3093 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 116:
#line 879 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { p = reenter(classtable, (yyvsp[-4].e)->sym);
			  sp->table->sym = p->sym;
			  p->info.typ->ref = sp->table;
			  p->info.typ->width = sp->offset;
			  p->info.typ->id = p->sym;
			  if (p->info.typ->base)
			  	sp->table->prev = (Table*)entry(classtable, p->info.typ->base)->info.typ->ref;
			  (yyval.typ) = p->info.typ;
			  exitscope();
			}
#line 3108 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 117:
#line 890 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { p = reenter(classtable, (yyvsp[-6].e)->sym);
			  sp->table->sym = p->sym;
			  if (!(yyvsp[-4].e))
				semerror("invalid base class");
			  else
			  {	sp->table->prev = (Table*)(yyvsp[-4].e)->info.typ->ref;
				if (!sp->table->prev && !(yyvsp[-4].e)->info.typ->transient)
				{	sprintf(errbuf, "class '%s' has incomplete type", (yyvsp[-4].e)->sym->name);
					semerror(errbuf);
				}
			  	p->info.typ->base = (yyvsp[-4].e)->info.typ->id;
			  }
			  p->info.typ->ref = sp->table;
			  p->info.typ->width = sp->offset;
			  p->info.typ->id = p->sym;
			  (yyval.typ) = p->info.typ;
			  exitscope();
			}
#line 3131 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 118:
#line 908 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyvsp[0].e)->info.typ->id = (yyvsp[0].e)->sym;
			  (yyval.typ) = (yyvsp[0].e)->info.typ;
			}
#line 3139 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 119:
#line 912 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if (!(yyvsp[0].e))
				semerror("invalid base class");
			  else
			  {	if (!(yyvsp[0].e)->info.typ->ref && !(yyvsp[0].e)->info.typ->transient)
				{	sprintf(errbuf, "class '%s' has incomplete type", (yyvsp[0].e)->sym->name);
					semerror(errbuf);
				}
			  	(yyvsp[-2].e)->info.typ->base = (yyvsp[0].e)->info.typ->id;
			  }
			  (yyvsp[-2].e)->info.typ->id = (yyvsp[-2].e)->sym;
			  (yyval.typ) = (yyvsp[-2].e)->info.typ;
			}
#line 3156 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 120:
#line 925 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { sym = gensym("_Struct");
			  sprintf(errbuf, "anonymous struct will be named '%s'", sym->name);
			  semwarn(errbuf);
			  if ((p = entry(classtable, sym)))
			  {	if (p->info.typ->ref || p->info.typ->type != Tstruct)
				{	sprintf(errbuf, "struct '%s' already declared at line %d", sym->name, p->lineno);
					semerror(errbuf);
				}
				else
				{	p->info.typ->ref = sp->table;
					p->info.typ->width = sp->offset;
				}
			  }
			  else
			  {	p = enter(classtable, sym);
				p->info.typ = mkstruct(sp->table, sp->offset);
			  }
			  p->info.typ->id = sym;
			  (yyval.typ) = p->info.typ;
			  exitscope();
			}
#line 3182 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 121:
#line 947 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((p = entry(classtable, (yyvsp[-4].e)->sym)) && p->info.typ->ref)
			  {	if (is_mutable(p->info.typ))
			  	{	if (merge((Table*)p->info.typ->ref, sp->table))
					{	sprintf(errbuf, "member name clash in struct '%s' declared at line %d", (yyvsp[-4].e)->sym->name, p->lineno);
						semerror(errbuf);
					}
			  		p->info.typ->width += sp->offset;
				}
			  }
			  else
			  {	p = reenter(classtable, (yyvsp[-4].e)->sym);
			  	p->info.typ->ref = sp->table;
			  	p->info.typ->width = sp->offset;
			  	p->info.typ->id = p->sym;
			  }
			  (yyval.typ) = p->info.typ;
			  exitscope();
			}
#line 3205 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 122:
#line 965 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((p = entry(classtable, (yyvsp[0].sym))))
			  {	if (p->info.typ->type == Tstruct)
			  		(yyval.typ) = p->info.typ;
			  	else
				{	sprintf(errbuf, "'struct %s' redeclaration (line %d)", (yyvsp[0].sym)->name, p->lineno);
			  		semerror(errbuf);
			  		(yyval.typ) = mkint();
				}
			  }
			  else
			  {	p = enter(classtable, (yyvsp[0].sym));
			  	(yyval.typ) = p->info.typ = mkstruct((Table*)0, 0);
				p->info.typ->id = (yyvsp[0].sym);
			  }
			}
#line 3225 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 123:
#line 980 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((p = entry(classtable, (yyvsp[0].sym))))
			  {	if (p->info.typ->type == Tstruct)
					(yyval.typ) = p->info.typ;
			  	else
				{	sprintf(errbuf, "'struct %s' redeclaration (line %d)", (yyvsp[0].sym)->name, p->lineno);
			  		semerror(errbuf);
			  		(yyval.typ) = mkint();
				}
			  }
			  else
			  {	p = enter(classtable, (yyvsp[0].sym));
			  	(yyval.typ) = p->info.typ = mkstruct((Table*)0, 0);
				p->info.typ->id = (yyvsp[0].sym);
			  }
			}
#line 3245 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 124:
#line 996 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { sym = gensym("_Union");
			  sprintf(errbuf, "anonymous union will be named '%s'", sym->name);
			  semwarn(errbuf);
			  (yyval.typ) = mkunion(sp->table, sp->offset);
			  if ((p = entry(classtable, sym)))
			  {	if (p->info.typ->ref)
				{	sprintf(errbuf, "union or struct '%s' already declared at line %d", sym->name, p->lineno);
					semerror(errbuf);
				}
				else
				{	p->info.typ->ref = sp->table;
					p->info.typ->width = sp->offset;
				}
			  }
			  else
			  {	p = enter(classtable, sym);
				p->info.typ = mkunion(sp->table, sp->offset);
			  }
			  p->info.typ->id = sym;
			  (yyval.typ) = p->info.typ;
			  exitscope();
			}
#line 3272 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 125:
#line 1019 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((p = entry(classtable, (yyvsp[-4].sym))))
			  {	if (p->info.typ->ref || p->info.typ->type != Tunion)
			  	{	sprintf(errbuf, "union '%s' already declared at line %d", (yyvsp[-4].sym)->name, p->lineno);
					semerror(errbuf);
				}
				else
				{	p = reenter(classtable, (yyvsp[-4].sym));
					p->info.typ->ref = sp->table;
					p->info.typ->width = sp->offset;
				}
			  }
			  else
			  {	p = enter(classtable, (yyvsp[-4].sym));
				p->info.typ = mkunion(sp->table, sp->offset);
			  }
			  p->info.typ->id = (yyvsp[-4].sym);
			  (yyval.typ) = p->info.typ;
			  exitscope();
			}
#line 3296 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 126:
#line 1038 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((p = entry(classtable, (yyvsp[0].sym))))
			  {	if (p->info.typ->type == Tunion)
					(yyval.typ) = p->info.typ;
			  	else
				{	sprintf(errbuf, "'union %s' redeclaration (line %d)", (yyvsp[0].sym)->name, p->lineno);
			  		semerror(errbuf);
			  		(yyval.typ) = mkint();
				}
			  }
			  else
			  {	p = enter(classtable, (yyvsp[0].sym));
			  	(yyval.typ) = p->info.typ = mkunion((Table*) 0, 0);
				p->info.typ->id = (yyvsp[0].sym);
			  }
			}
#line 3316 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 127:
#line 1053 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((p = entry(classtable, (yyvsp[0].sym))))
			  {	if (p->info.typ->type == Tunion)
					(yyval.typ) = p->info.typ;
			  	else
				{	sprintf(errbuf, "'union %s' redeclaration (line %d)", (yyvsp[0].sym)->name, p->lineno);
			  		semerror(errbuf);
			  		(yyval.typ) = mkint();
				}
			  }
			  else
			  {	p = enter(classtable, (yyvsp[0].sym));
			  	(yyval.typ) = p->info.typ = mkunion((Table*) 0, 0);
				p->info.typ->id = (yyvsp[0].sym);
			  }
			}
#line 3336 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 128:
#line 1069 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { sym = gensym("_Enum");
			  sprintf(errbuf, "anonymous enum will be named '%s'", sym->name);
			  semwarn(errbuf);
			  if ((p = entry(enumtable, sym)))
			  {	if (p->info.typ->ref)
				{	sprintf(errbuf, "enum '%s' already declared at line %d", sym->name, p->lineno);
					semerror(errbuf);
				}
				else
				{	p->info.typ->ref = sp->table;
					p->info.typ->width = 4; /* 4 = enum */
				}
			  }
			  else
			  {	p = enter(enumtable, sym);
				p->info.typ = mkenum(sp->table);
			  }
			  p->info.typ->id = sym;
			  (yyval.typ) = p->info.typ;
			  exitscope();
			}
#line 3362 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 129:
#line 1091 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((p = entry(enumtable, (yyvsp[-5].e)->sym)))
			  {	if (p->info.typ->ref)
				{	sprintf(errbuf, "enum '%s' already declared at line %d", (yyvsp[-5].e)->sym->name, p->lineno);
					semerror(errbuf);
				}
				else
				{	p->info.typ->ref = sp->table;
					p->info.typ->width = 4; /* 4 = enum */
				}
			  }
			  else
			  {	p = enter(enumtable, (yyvsp[-5].e)->sym);
				p->info.typ = mkenum(sp->table);
			  }
			  p->info.typ->id = (yyvsp[-5].e)->sym;
			  (yyval.typ) = p->info.typ;
			  exitscope();
			}
#line 3385 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 130:
#line 1110 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((p = entry(enumtable, (yyvsp[-5].sym))))
			  {	if (p->info.typ->ref)
				{	sprintf(errbuf, "enum '%s' already declared at line %d", (yyvsp[-5].sym)->name, p->lineno);
					semerror(errbuf);
				}
				else
				{	p->info.typ->ref = sp->table;
					p->info.typ->width = 8; /* 8 = mask */
				}
			  }
			  else
			  {	p = enter(enumtable, (yyvsp[-5].sym));
				p->info.typ = mkmask(sp->table);
			  }
			  p->info.typ->id = (yyvsp[-5].sym);
			  (yyval.typ) = p->info.typ;
			  exitscope();
			}
#line 3408 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 131:
#line 1128 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((p = entry(enumtable, (yyvsp[0].sym))))
			  	(yyval.typ) = p->info.typ;
			  else
			  {	p = enter(enumtable, (yyvsp[0].sym));
			  	(yyval.typ) = p->info.typ = mkenum((Table*)0);
				p->info.typ->id = (yyvsp[0].sym);
			  }
			}
#line 3421 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 132:
#line 1136 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((p = entry(enumtable, (yyvsp[0].sym))))
				(yyval.typ) = p->info.typ;
			  else
			  {	p = enter(enumtable, (yyvsp[0].sym));
			  	(yyval.typ) = p->info.typ = mkenum((Table*)0);
				p->info.typ->id = (yyvsp[0].sym);
			  }
			}
#line 3434 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 133:
#line 1144 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((p = entry(typetable, (yyvsp[0].sym))))
			  	(yyval.typ) = p->info.typ;
			  else if ((p = entry(classtable, (yyvsp[0].sym))))
			  	(yyval.typ) = p->info.typ;
			  else if ((p = entry(enumtable, (yyvsp[0].sym))))
			  	(yyval.typ) = p->info.typ;
			  else if ((yyvsp[0].sym) == lookup("std::string") || (yyvsp[0].sym) == lookup("std::wstring"))
			  {	p = enter(classtable, (yyvsp[0].sym));
				(yyval.typ) = p->info.typ = mkclass((Table*)0, 0);
			  	p->info.typ->id = (yyvsp[0].sym);
				if (cflag)
			  		p->info.typ->transient = 1;	/* make std::string transient in C */
				else
			  		p->info.typ->transient = -2;
			  }
			  else
			  {	sprintf(errbuf, "unknown type '%s'", (yyvsp[0].sym)->name);
				semerror(errbuf);
				(yyval.typ) = mkint();
			  }
			}
#line 3460 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 134:
#line 1166 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((p = entry(templatetable, (yyvsp[-3].sym))))
			  {	(yyval.typ) = mktemplate((yyvsp[-1].rec).typ, (yyvsp[-3].sym));
			  	if (has_attachment((yyvsp[-1].rec).typ))
				{	sprintf(errbuf, "template type '%s<%s>' of attachment objects may lead to deserialization failures, use '%s<*%s>' instead", (yyvsp[-3].sym)->name, (yyvsp[-1].rec).typ->id ? (yyvsp[-1].rec).typ->id->name : "", (yyvsp[-3].sym)->name, (yyvsp[-1].rec).typ->id ? (yyvsp[-1].rec).typ->id->name : "");
					semwarn(errbuf);
				}
			  }
			  else
			  {	sprintf(errbuf, "invalid template '%s'", (yyvsp[-3].sym)->name);
				semerror(errbuf);
				(yyval.typ) = mkint();
			  }
			}
#line 3478 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 135:
#line 1180 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { synerror("malformed class definition (use spacing around ':' to separate derived : base)");
			  yyerrok;
			  (yyval.typ) = mkint();
			}
#line 3487 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 136:
#line 1185 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { synerror("malformed struct definition");
			  yyerrok;
			  (yyval.typ) = mkint();
			}
#line 3496 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 137:
#line 1190 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { synerror("malformed union definition");
			  yyerrok;
			  (yyval.typ) = mkint();
			}
#line 3505 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 138:
#line 1195 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { synerror("malformed enum definition");
			  yyerrok;
			  (yyval.typ) = mkint();
			}
#line 3514 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 139:
#line 1200 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((p = entry(classtable, (yyvsp[0].sym))))
			  {	if (p->info.typ->ref)
			   	{	if (!is_mutable(p->info.typ))
					{	sprintf(errbuf, "struct '%s' already declared at line %d", (yyvsp[0].sym)->name, p->lineno);
						semerror(errbuf);
					}
				}
				else
					p = reenter(classtable, (yyvsp[0].sym));
			  }
			  else
			  {	p = enter(classtable, (yyvsp[0].sym));
				p->info.typ = mkstruct((Table*)0, 0);
			  }
			  (yyval.e) = p;
			}
#line 3535 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 140:
#line 1217 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((p = entry(classtable, (yyvsp[0].sym))))
			  {	if (p->info.typ->ref)
			   	{	if (!is_mutable(p->info.typ))
					{	sprintf(errbuf, "class '%s' already declared at line %d (redundant 'class' specifier here?)", (yyvsp[0].sym)->name, p->lineno);
						semerror(errbuf);
					}
				}
				else
					p = reenter(classtable, (yyvsp[0].sym));
			  }
			  else
			  {	p = enter(classtable, (yyvsp[0].sym));
				p->info.typ = mkclass((Table*)0, 0);
				p->info.typ->id = p->sym;
			  }
			  (yyvsp[0].sym)->token = TYPE;
			  (yyval.e) = p;
			}
#line 3558 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 141:
#line 1236 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((p = entry(enumtable, (yyvsp[0].sym))))
			  {	if (p->info.typ->ref)
				{	sprintf(errbuf, "enum '%s' already declared at line %d", (yyvsp[0].sym)->name, p->lineno);
					semerror(errbuf);
				}
				/*
				else
					p = reenter(classtable, $2);
			  	*/
			  }
			  else
			  {	p = enter(enumtable, (yyvsp[0].sym));
				p->info.typ = mkenum(0);
			  }
			  (yyval.e) = p;
			}
#line 3579 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 142:
#line 1253 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 3585 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 143:
#line 1254 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 3591 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 144:
#line 1256 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.e) = (yyvsp[0].e); }
#line 3597 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 145:
#line 1257 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.e) = (yyvsp[0].e); }
#line 3603 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 146:
#line 1258 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.e) = (yyvsp[0].e); }
#line 3609 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 147:
#line 1259 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.e) = entry(classtable, (yyvsp[0].sym));
			  if (!(yyval.e))
			  {	p = entry(typetable, (yyvsp[0].sym));
			  	if (p && (p->info.typ->type == Tclass || p->info.typ->type == Tstruct))
					(yyval.e) = p;
			  }
			}
#line 3621 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 148:
#line 1266 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.e) = entry(classtable, (yyvsp[0].sym)); }
#line 3627 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 149:
#line 1268 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if (transient == -2)
			  	transient = 0;
			  permission = 0;
			  enterscope(mktable(NULL), 0);
			  sp->entry = NULL;
			}
#line 3638 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 150:
#line 1275 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if (transient == -2)
			  	transient = 0;
			  permission = 0;
			  enterscope(mktable(NULL), 0);
			  sp->entry = NULL;
			  sp->grow = False;
			}
#line 3650 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 151:
#line 1283 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { enterscope(mktable(NULL), 0);
			  sp->entry = NULL;
			  sp->mask = True;
			  sp->val = 1;
			}
#line 3660 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 152:
#line 1289 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 3666 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 153:
#line 1290 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { }
#line 3672 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 154:
#line 1292 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if (sp->table->level == INTERNAL)
			  	transient |= 1;
			  permission = 0;
			  enterscope(mktable(NULL), 0);
			  sp->entry = NULL;
			  sp->table->level = PARAM;
			}
#line 3684 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 155:
#line 1300 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = Sauto; }
#line 3690 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 156:
#line 1301 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = Sregister; }
#line 3696 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 157:
#line 1302 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = Sstatic; }
#line 3702 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 158:
#line 1303 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = Sexplicit; }
#line 3708 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 159:
#line 1304 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = Sextern; transient = 1; }
#line 3714 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 160:
#line 1305 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = Stypedef; }
#line 3720 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 161:
#line 1306 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = Svirtual; }
#line 3726 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 162:
#line 1307 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = Sconst; }
#line 3732 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 163:
#line 1308 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = Sfriend; }
#line 3738 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 164:
#line 1309 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = Sinline; }
#line 3744 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 165:
#line 1310 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = SmustUnderstand; }
#line 3750 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 166:
#line 1311 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = Sreturn; }
#line 3756 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 167:
#line 1312 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = Sattribute;
			  if (eflag)
			   	semwarn("SOAP RPC encoding does not support XML attributes");
			}
#line 3765 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 168:
#line 1316 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = Sspecial; }
#line 3771 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 169:
#line 1317 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = Sextern; transient = -2; }
#line 3777 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 170:
#line 1319 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = Snone; }
#line 3783 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 171:
#line 1320 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = Sconstobj; }
#line 3789 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 172:
#line 1322 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = Snone; }
#line 3795 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 173:
#line 1323 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = Sabstract; }
#line 3801 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 174:
#line 1325 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = Snone; }
#line 3807 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 175:
#line 1326 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.sto) = Svirtual; }
#line 3813 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 176:
#line 1328 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = tmp = sp->node; }
#line 3819 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 177:
#line 1329 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { /* handle const pointers, such as const char* */
			  if ((tmp.sto & Sconst))
			  	tmp.sto = (Storage)(((int)tmp.sto & ~Sconst) | Sconstptr);
			  tmp.typ = mkpointer(tmp.typ);
			  tmp.typ->transient = transient;
			  (yyval.rec) = tmp;
			}
#line 3831 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 178:
#line 1336 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { tmp.typ = mkreference(tmp.typ);
			  tmp.typ->transient = transient;
			  (yyval.rec) = tmp;
			}
#line 3840 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 179:
#line 1341 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = tmp;	/* tmp is inherited */
			}
#line 3847 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 180:
#line 1344 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if (!bflag && (yyvsp[0].rec).typ->type == Tchar)
			  {	sprintf(errbuf, "char[" SOAP_LONG_FORMAT "] will be serialized as an array of " SOAP_LONG_FORMAT " bytes: use soapcpp2 option -b to enable char[] string serialization or use char* for strings", (yyvsp[-2].rec).val.i, (yyvsp[-2].rec).val.i);
			  	semwarn(errbuf);
			  }
			  if ((yyvsp[-2].rec).hasval && (yyvsp[-2].rec).typ->type == Tint && (yyvsp[-2].rec).val.i > 0 && (yyvsp[0].rec).typ->width > 0)
				(yyval.rec).typ = mkarray((yyvsp[0].rec).typ, (int) (yyvsp[-2].rec).val.i * (yyvsp[0].rec).typ->width);
			  else
			  {	(yyval.rec).typ = mkarray((yyvsp[0].rec).typ, 0);
			  	semerror("undetermined array size");
			  }
			  (yyval.rec).sto = (yyvsp[0].rec).sto;
			}
#line 3864 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 181:
#line 1356 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).typ = mkpointer((yyvsp[0].rec).typ); /* zero size array = pointer */
			  (yyval.rec).sto = (yyvsp[0].rec).sto;
			}
#line 3872 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 182:
#line 1360 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((yyvsp[0].rec).typ->type == Tstruct || (yyvsp[0].rec).typ->type == Tclass)
				if (!(yyvsp[0].rec).typ->ref && !(yyvsp[0].rec).typ->transient && !((yyvsp[0].rec).sto & Stypedef))
			   	{	sprintf(errbuf, "struct/class '%s' has incomplete type", (yyvsp[0].rec).typ->id->name);
					semerror(errbuf);
				}
			  (yyval.rec) = (yyvsp[0].rec);
			}
#line 3884 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 183:
#line 1368 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).hasval = False; }
#line 3890 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 184:
#line 1369 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((yyvsp[0].rec).hasval)
			  {	(yyval.rec).typ = (yyvsp[0].rec).typ;
				(yyval.rec).hasval = True;
				(yyval.rec).val = (yyvsp[0].rec).val;
			  }
			  else
			  {	(yyval.rec).hasval = False;
				semerror("initialization expression not constant");
			  }
			}
#line 3905 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 185:
#line 1380 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.s) = NULL; }
#line 3911 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 186:
#line 1381 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3917 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 187:
#line 1384 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).minOccurs = -1;
			  (yyval.rec).maxOccurs = 1;
			  (yyval.rec).minLength = MINLONG64;
			  (yyval.rec).maxLength = MAXLONG64;
			  (yyval.rec).pattern = (yyvsp[0].s);
			}
#line 3928 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 188:
#line 1391 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).minOccurs = (LONG64)(yyvsp[0].i);
			  (yyval.rec).maxOccurs = 1;
			  (yyval.rec).minLength = (LONG64)(yyvsp[0].i);
			  (yyval.rec).maxLength = MAXLONG64;
			  (yyval.rec).pattern = (yyvsp[-1].s);
			}
#line 3939 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 189:
#line 1398 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).minOccurs = (LONG64)(yyvsp[-1].i);
			  (yyval.rec).maxOccurs = 1;
			  (yyval.rec).minLength = (LONG64)(yyvsp[-1].i);
			  (yyval.rec).maxLength = MAXLONG64;
			  (yyval.rec).pattern = (yyvsp[-2].s);
			}
#line 3950 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 190:
#line 1405 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).minOccurs = (LONG64)(yyvsp[-2].i);
			  (yyval.rec).maxOccurs = (LONG64)(yyvsp[0].i);
			  (yyval.rec).minLength = (LONG64)(yyvsp[-2].i);
			  (yyval.rec).maxLength = (LONG64)(yyvsp[0].i);
			  (yyval.rec).pattern = (yyvsp[-3].s);
			}
#line 3961 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 191:
#line 1412 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).minOccurs = -1;
			  (yyval.rec).maxOccurs = (LONG64)(yyvsp[0].i);
			  (yyval.rec).minLength = MINLONG64;
			  (yyval.rec).maxLength = (LONG64)(yyvsp[0].i);
			  (yyval.rec).pattern = (yyvsp[-2].s);
			}
#line 3972 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 192:
#line 1419 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.s) = NULL; }
#line 3978 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 193:
#line 1420 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.s) = (yyvsp[0].s); }
#line 3984 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 194:
#line 1422 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i); }
#line 3990 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 195:
#line 1423 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[0].i); }
#line 3996 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 196:
#line 1424 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.i) = -(yyvsp[0].i); }
#line 4002 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 197:
#line 1433 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = (yyvsp[0].rec); }
#line 4008 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 198:
#line 1434 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = (yyvsp[0].rec); }
#line 4014 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 199:
#line 1438 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).typ = (yyvsp[-2].rec).typ;
			  (yyval.rec).sto = Snone;
			  (yyval.rec).hasval = False;
			}
#line 4023 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 201:
#line 1445 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = (yyvsp[0].rec); }
#line 4029 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 202:
#line 1448 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).hasval = False;
			  (yyval.rec).typ = mkint();
			}
#line 4037 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 203:
#line 1451 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = (yyvsp[0].rec); }
#line 4043 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 204:
#line 1453 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = (yyvsp[0].rec); }
#line 4049 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 205:
#line 1456 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).hasval = False;
			  (yyval.rec).typ = mkint();
			}
#line 4057 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 206:
#line 1459 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = (yyvsp[0].rec); }
#line 4063 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 207:
#line 1461 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = (yyvsp[0].rec); }
#line 4069 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 208:
#line 1464 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = iop("|", (yyvsp[-2].rec), (yyvsp[0].rec)); }
#line 4075 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 209:
#line 1465 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = iop("^", (yyvsp[-2].rec), (yyvsp[0].rec)); }
#line 4081 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 210:
#line 1466 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = iop("&", (yyvsp[-2].rec), (yyvsp[0].rec)); }
#line 4087 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 211:
#line 1467 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = relop("==", (yyvsp[-2].rec), (yyvsp[0].rec)); }
#line 4093 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 212:
#line 1468 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = relop("!=", (yyvsp[-2].rec), (yyvsp[0].rec)); }
#line 4099 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 213:
#line 1469 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = relop("<", (yyvsp[-2].rec), (yyvsp[0].rec)); }
#line 4105 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 214:
#line 1470 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = relop("<=", (yyvsp[-2].rec), (yyvsp[0].rec)); }
#line 4111 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 215:
#line 1471 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = relop(">", (yyvsp[-2].rec), (yyvsp[0].rec)); }
#line 4117 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 216:
#line 1472 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = relop(">=", (yyvsp[-2].rec), (yyvsp[0].rec)); }
#line 4123 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 217:
#line 1473 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = iop("<<", (yyvsp[-2].rec), (yyvsp[0].rec)); }
#line 4129 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 218:
#line 1474 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = iop(">>", (yyvsp[-2].rec), (yyvsp[0].rec)); }
#line 4135 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 219:
#line 1475 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = op("+", (yyvsp[-2].rec), (yyvsp[0].rec)); }
#line 4141 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 220:
#line 1476 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = op("-", (yyvsp[-2].rec), (yyvsp[0].rec)); }
#line 4147 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 221:
#line 1477 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = op("*", (yyvsp[-2].rec), (yyvsp[0].rec)); }
#line 4153 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 222:
#line 1478 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = op("/", (yyvsp[-2].rec), (yyvsp[0].rec)); }
#line 4159 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 223:
#line 1479 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = iop("%", (yyvsp[-2].rec), (yyvsp[0].rec)); }
#line 4165 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 224:
#line 1480 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = (yyvsp[0].rec); }
#line 4171 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 225:
#line 1483 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((yyvsp[0].rec).hasval)
				(yyval.rec).val.i = !(yyvsp[0].rec).val.i;
			  (yyval.rec).typ = (yyvsp[0].rec).typ;
			  (yyval.rec).hasval = (yyvsp[0].rec).hasval;
			}
#line 4181 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 226:
#line 1488 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((yyvsp[0].rec).hasval)
				(yyval.rec).val.i = ~(yyvsp[0].rec).val.i;
			  (yyval.rec).typ = (yyvsp[0].rec).typ;
			  (yyval.rec).hasval = (yyvsp[0].rec).hasval;
			}
#line 4191 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 227:
#line 1493 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((yyvsp[0].rec).hasval) {
				if (integer((yyvsp[0].rec).typ))
					(yyval.rec).val.i = -(yyvsp[0].rec).val.i;
				else if (real((yyvsp[0].rec).typ))
					(yyval.rec).val.r = -(yyvsp[0].rec).val.r;
				else	typerror("string?");
			  }
			  (yyval.rec).typ = (yyvsp[0].rec).typ;
			  (yyval.rec).hasval = (yyvsp[0].rec).hasval;
			}
#line 4206 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 228:
#line 1503 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = (yyvsp[0].rec); }
#line 4212 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 229:
#line 1504 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((yyvsp[0].rec).typ->type == Tpointer) {
			  	(yyval.rec).typ = (Tnode*)(yyvsp[0].rec).typ->ref;
			  } else
			  	typerror("dereference of non-pointer type");
			  (yyval.rec).sto = Snone;
			  (yyval.rec).hasval = False;
			}
#line 4224 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 230:
#line 1511 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).typ = mkpointer((yyvsp[0].rec).typ);
			  (yyval.rec).sto = Snone;
			  (yyval.rec).hasval = False;
			}
#line 4233 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 231:
#line 1516 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).hasval = True;
			  (yyval.rec).typ = mkint();
			  (yyval.rec).val.i = (yyvsp[-1].rec).typ->width;
			}
#line 4242 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 232:
#line 1520 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = (yyvsp[0].rec); }
#line 4248 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 233:
#line 1523 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec) = (yyvsp[-1].rec); }
#line 4254 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 234:
#line 1524 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { if ((p = enumentry((yyvsp[0].sym))) == (Entry*) 0)
				p = undefined((yyvsp[0].sym));
			  else
			  	(yyval.rec).hasval = True;
			  (yyval.rec).typ = p->info.typ;
			  (yyval.rec).val = p->info.val;
			}
#line 4266 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 235:
#line 1531 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).typ = mkint();
			  (yyval.rec).hasval = True;
			  (yyval.rec).val.i = (yyvsp[0].i);
			}
#line 4275 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 236:
#line 1535 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).typ = mkint();
			  (yyval.rec).hasval = True;
			  (yyval.rec).val.i = 0;
			}
#line 4284 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 237:
#line 1539 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).typ = mkfloat();
			  (yyval.rec).hasval = True;
			  (yyval.rec).val.r = (yyvsp[0].r);
			}
#line 4293 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 238:
#line 1543 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).typ = mkchar();
			  (yyval.rec).hasval = True;
			  (yyval.rec).val.i = (yyvsp[0].c);
			}
#line 4302 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 239:
#line 1547 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).typ = mkstring();
			  (yyval.rec).hasval = True;
			  (yyval.rec).val.s = (yyvsp[0].s);
			}
#line 4311 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 240:
#line 1551 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).typ = mkbool();
			  (yyval.rec).hasval = True;
			  (yyval.rec).val.i = 0;
			}
#line 4320 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;

  case 241:
#line 1555 "soapcpp2_yacc.y" /* yacc.c:1646  */
    { (yyval.rec).typ = mkbool();
			  (yyval.rec).hasval = True;
			  (yyval.rec).val.i = 1;
			}
#line 4329 "soapcpp2_yacc.c" /* yacc.c:1646  */
    break;


#line 4333 "soapcpp2_yacc.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1561 "soapcpp2_yacc.y" /* yacc.c:1906  */


/*
 * ???
 */
int
yywrap(void)
{	return 1;
}

/******************************************************************************\

	Support routines

\******************************************************************************/

static Node
op(const char *op, Node p, Node q)
{	Node	r;
	Tnode	*typ;
	r.typ = p.typ;
	r.sto = Snone;
	if (p.hasval && q.hasval) {
		if (integer(p.typ) && integer(q.typ))
			switch (op[0]) {
			case '|':	r.val.i = p.val.i |  q.val.i; break;
			case '^':	r.val.i = p.val.i ^  q.val.i; break;
			case '&':	r.val.i = p.val.i &  q.val.i; break;
			case '<':	r.val.i = p.val.i << q.val.i; break;
			case '>':	r.val.i = p.val.i >> q.val.i; break;
			case '+':	r.val.i = p.val.i +  q.val.i; break;
			case '-':	r.val.i = p.val.i -  q.val.i; break;
			case '*':	r.val.i = p.val.i *  q.val.i; break;
			case '/':	r.val.i = p.val.i /  q.val.i; break;
			case '%':	r.val.i = p.val.i %  q.val.i; break;
			default:	typerror(op);
			}
		else if (real(p.typ) && real(q.typ))
			switch (op[0]) {
			case '+':	r.val.r = p.val.r + q.val.r; break;
			case '-':	r.val.r = p.val.r - q.val.r; break;
			case '*':	r.val.r = p.val.r * q.val.r; break;
			case '/':	r.val.r = p.val.r / q.val.r; break;
			default:	typerror(op);
			}
		else	semerror("illegal constant operation");
		r.hasval = True;
	} else {
		typ = mgtype(p.typ, q.typ);
		r.hasval = False;
	}
	return r;
}

static Node
iop(const char *iop, Node p, Node q)
{	if (integer(p.typ) && integer(q.typ))
		return op(iop, p, q);
	typerror("integer operands only");
	return p;
}

static Node
relop(const char *op, Node p, Node q)
{	Node	r;
	Tnode	*typ;
	r.typ = mkint();
	r.sto = Snone;
	r.hasval = False;
	if (p.typ->type != Tpointer || p.typ != q.typ)
		typ = mgtype(p.typ, q.typ);
	return r;
}

/******************************************************************************\

	Scope management

\******************************************************************************/

/*
mkscope - initialize scope stack with a new table and offset
*/
static void
mkscope(Table *table, int offset)
{	sp = stack-1;
	enterscope(table, offset);
}

/*
enterscope - enter a new scope by pushing a new table and offset on the stack
*/
static void
enterscope(Table *table, int offset)
{	if (++sp == stack+MAXNEST)
		execerror("maximum scope depth exceeded");
	sp->table = table;
	sp->val = 0;
	sp->offset = offset;
	sp->grow = True;	/* by default, offset grows */
	sp->mask = False;
}

/*
exitscope - exit a scope by popping the table and offset from the stack
*/
static void
exitscope(void)
{	check(sp-- != stack, "exitscope() has no matching enterscope()");
}

/******************************************************************************\

	Undefined symbol

\******************************************************************************/

static Entry*
undefined(Symbol *sym)
{	Entry	*p;
	sprintf(errbuf, "undefined identifier '%s'", sym->name);
	semwarn(errbuf);
	p = enter(sp->table, sym);
	p->level = GLOBAL;
	p->info.typ = mkint();
	p->info.sto = Sextern;
	p->info.hasval = False;
	return p;
}

/*
mgtype - return most general type among two numerical types
*/
Tnode*
mgtype(Tnode *typ1, Tnode *typ2)
{	if (numeric(typ1) && numeric(typ2)) {
		if (typ1->type < typ2->type)
			return typ2;
	} else	typerror("non-numeric type");
	return typ1;
}

/******************************************************************************\

	Type checks

\******************************************************************************/

static int
integer(Tnode *typ)
{	switch (typ->type) {
	case Tchar:
	case Tshort:
	case Tint:
	case Tlong:	return True;
	default:	break;
	}
	return False;
}

static int
real(Tnode *typ)
{	switch (typ->type) {
	case Tfloat:
	case Tdouble:
	case Tldouble:	return True;
	default:	break;
	}
	return False;
}

static int
numeric(Tnode *typ)
{	return integer(typ) || real(typ);
}

static void
add_fault(Table *gt)
{ Table *t;
  Entry *p1, *p2, *p3, *p4;
  Symbol *s1, *s2, *s3, *s4;
  imported = NULL;
  s1 = lookup("SOAP_ENV__Code");
  p1 = entry(classtable, s1);
  if (!p1 || !p1->info.typ->ref)
  { t = mktable((Table*)0);
    if (!p1)
    { p1 = enter(classtable, s1);
      p1->info.typ = mkstruct(t, 3*4);
      p1->info.typ->id = s1;
    }
    else
      p1->info.typ->ref = t;
    p2 = enter(t, lookup("SOAP_ENV__Value"));
    p2->info.typ = qname;
    p2->info.minOccurs = 0;
    p2 = enter(t, lookup("SOAP_ENV__Subcode"));
    p2->info.typ = mkpointer(p1->info.typ);
    p2->info.minOccurs = 0;
  }
  s2 = lookup("SOAP_ENV__Detail");
  p2 = entry(classtable, s2);
  if (!p2 || !p2->info.typ->ref)
  { t = mktable((Table*)0);
    if (!p2)
    { p2 = enter(classtable, s2);
      p2->info.typ = mkstruct(t, 3*4);
      p2->info.typ->id = s2;
    }
    else
      p2->info.typ->ref = t;
    p3 = enter(t, lookup("__any"));
    p3->info.typ = xml;
    p3->info.minOccurs = 0;
    p3 = enter(t, lookup("__type"));
    p3->info.typ = mkint();
    p3->info.minOccurs = 0;
    p3 = enter(t, lookup("fault"));
    p3->info.typ = mkpointer(mkvoid());
    p3->info.minOccurs = 0;
    custom_fault = 0;
  }
  s4 = lookup("SOAP_ENV__Reason");
  p4 = entry(classtable, s4);
  if (!p4 || !p4->info.typ->ref)
  { t = mktable((Table*)0);
    if (!p4)
    { p4 = enter(classtable, s4);
      p4->info.typ = mkstruct(t, 4);
      p4->info.typ->id = s4;
    }
    else
      p4->info.typ->ref = t;
    p3 = enter(t, lookup("SOAP_ENV__Text"));
    p3->info.typ = mkstring();
    p3->info.minOccurs = 0;
  }
  s3 = lookup("SOAP_ENV__Fault");
  p3 = entry(classtable, s3);
  if (!p3 || !p3->info.typ->ref)
  { t = mktable(NULL);
    if (!p3)
    { p3 = enter(classtable, s3);
      p3->info.typ = mkstruct(t, 9*4);
      p3->info.typ->id = s3;
    }
    else
      p3->info.typ->ref = t;
    p3 = enter(t, lookup("faultcode"));
    p3->info.typ = qname;
    p3->info.minOccurs = 0;
    p3 = enter(t, lookup("faultstring"));
    p3->info.typ = mkstring();
    p3->info.minOccurs = 0;
    p3 = enter(t, lookup("faultactor"));
    p3->info.typ = mkstring();
    p3->info.minOccurs = 0;
    p3 = enter(t, lookup("detail"));
    p3->info.typ = mkpointer(p2->info.typ);
    p3->info.minOccurs = 0;
    p3 = enter(t, s1);
    p3->info.typ = mkpointer(p1->info.typ);
    p3->info.minOccurs = 0;
    p3 = enter(t, s4);
    p3->info.typ = mkpointer(p4->info.typ);
    p3->info.minOccurs = 0;
    p3 = enter(t, lookup("SOAP_ENV__Node"));
    p3->info.typ = mkstring();
    p3->info.minOccurs = 0;
    p3 = enter(t, lookup("SOAP_ENV__Role"));
    p3->info.typ = mkstring();
    p3->info.minOccurs = 0;
    p3 = enter(t, lookup("SOAP_ENV__Detail"));
    p3->info.typ = mkpointer(p2->info.typ);
    p3->info.minOccurs = 0;
  }
}

static void
add_soap(void)
{ Symbol *s = lookup("soap");
  p = enter(classtable, s);
  p->info.typ = mkstruct(NULL, 0);
  p->info.typ->transient = -2;
  p->info.typ->id = s;
}

static void
add_XML(void)
{ Symbol *s = lookup("_XML");
  s->token = TYPE;
  p = enter(typetable, s);
  xml = p->info.typ = mksymtype(mkstring(), s);
  p->info.sto = Stypedef;
}

static void
add_qname(void)
{ Symbol *s = lookup("_QName");
  s->token = TYPE;
  p = enter(typetable, s);
  qname = p->info.typ = mksymtype(mkstring(), s);
  p->info.sto = Stypedef;
}

static void
add_header(Table *gt)
{ Table *t;
  Entry *p;
  Symbol *s = lookup("SOAP_ENV__Header");
  imported = NULL;
  p = entry(classtable, s);
  if (!p || !p->info.typ->ref)
  { t = mktable((Table*)0);
    if (!p)
      p = enter(classtable, s);
    p->info.typ = mkstruct(t, 0);
    p->info.typ->id = s;
    custom_header = 0;
  }
}

static void
add_response(Entry *fun, Entry *ret)
{ Table *t;
  Entry *p, *q;
  Symbol *s;
  size_t i = 0, j, n = strlen(fun->sym->name);
  char *r = (char*)emalloc(n+100);
  strcpy(r, fun->sym->name);
  strcat(r, "Response");
  do
  { for (j = 0; j < i; j++)
      r[n+j+8] = '_';
    r[n+i+8] = '\0';
    if (!(s = lookup(r)))
      s = install(r, ID);
    i++;
  } while (entry(classtable, s));
  free(r);
  t = mktable((Table*)0);
  q = enter(t, ret->sym);
  q->info = ret->info;
  if (q->info.typ->type == Treference)
    q->info.typ = (Tnode*)q->info.typ->ref;
  p = enter(classtable, s);
  p->info.typ = mkstruct(t, 4);
  p->info.typ->id = s;
  fun->info.typ->response = p;
}

static void
add_result(Tnode *typ)
{ Entry *p;
  if (!typ->ref || !((Tnode*)typ->ref)->ref)
  { semwarn("response struct/class must be declared before used in function prototype");
    return;
  }
  for (p = ((Table*)((Tnode*)typ->ref)->ref)->list; p; p = p->next)
    if (p->info.sto & Sreturn)
      return;
  for (p = ((Table*)((Tnode*)typ->ref)->ref)->list; p; p = p->next)
  { if (p->info.typ->type != Tfun && !(p->info.sto & Sattribute) && !is_transient(p->info.typ) && !(p->info.sto & (Sprivate|Sprotected)))
      p->info.sto = (Storage)((int)p->info.sto | (int)Sreturn);
      return;
  }
}
