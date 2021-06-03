/* A Bison parser, made by GNU Bison 3.7.6.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30706

/* Bison version string.  */
#define YYBISON_VERSION "3.7.6"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.ypp"

/* Declarations section */
    #include "SemanticsHandler.h"
    #include "hw3_output.hpp"

    extern int yylex();
    extern int yylineno;
    int yyerror(const char * message);

#line 81 "parser.tab.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_VOID = 3,                       /* VOID  */
  YYSYMBOL_INT = 4,                        /* INT  */
  YYSYMBOL_BYTE = 5,                       /* BYTE  */
  YYSYMBOL_B = 6,                          /* B  */
  YYSYMBOL_BOOL = 7,                       /* BOOL  */
  YYSYMBOL_TRUE = 8,                       /* TRUE  */
  YYSYMBOL_FALSE = 9,                      /* FALSE  */
  YYSYMBOL_RETURN = 10,                    /* RETURN  */
  YYSYMBOL_IF = 11,                        /* IF  */
  YYSYMBOL_ELSE = 12,                      /* ELSE  */
  YYSYMBOL_WHILE = 13,                     /* WHILE  */
  YYSYMBOL_BREAK = 14,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 15,                  /* CONTINUE  */
  YYSYMBOL_SC = 16,                        /* SC  */
  YYSYMBOL_COMMA = 17,                     /* COMMA  */
  YYSYMBOL_ID = 18,                        /* ID  */
  YYSYMBOL_NUM = 19,                       /* NUM  */
  YYSYMBOL_STRING = 20,                    /* STRING  */
  YYSYMBOL_ASSIGN = 21,                    /* ASSIGN  */
  YYSYMBOL_OR = 22,                        /* OR  */
  YYSYMBOL_AND = 23,                       /* AND  */
  YYSYMBOL_EQ = 24,              /* EQ  */
  YYSYMBOL_REL = 25,                 /* REL  */
  YYSYMBOL_PLUS_MINUS = 26,             /* PLUS_MINUS  */
  YYSYMBOL_MUL_DIV = 27,             /* MUL_DIV  */
  YYSYMBOL_NOT = 28,                       /* NOT  */
  YYSYMBOL_SWITCH = 29,                    /* SWITCH  */
  YYSYMBOL_CASE = 30,                      /* CASE  */
  YYSYMBOL_DEFAULT = 31,                   /* DEFAULT  */
  YYSYMBOL_COLON = 32,                     /* COLON  */
  YYSYMBOL_RBRACE = 33,                    /* RBRACE  */
  YYSYMBOL_LBRACE = 34,                    /* LBRACE  */
  YYSYMBOL_RPAREN = 35,                    /* RPAREN  */
  YYSYMBOL_LPAREN = 36,                    /* LPAREN  */
  YYSYMBOL_SECOND_PRIOR = 37,              /* SECOND_PRIOR  */
  YYSYMBOL_FIRST_PRIOR = 38,               /* FIRST_PRIOR  */
  YYSYMBOL_YYACCEPT = 39,                  /* $accept  */
  YYSYMBOL_Program = 40,                   /* Program  */
  YYSYMBOL_41_1 = 41,                      /* @1  */
  YYSYMBOL_Funcs = 42,                     /* Funcs  */
  YYSYMBOL_FuncDecl = 43,                  /* FuncDecl  */
  YYSYMBOL_44_2 = 44,                      /* @2  */
  YYSYMBOL_45_3 = 45,                      /* $@3  */
  YYSYMBOL_46_4 = 46,                      /* $@4  */
  YYSYMBOL_RetType = 47,                   /* RetType  */
  YYSYMBOL_Formals = 48,                   /* Formals  */
  YYSYMBOL_FormalsList = 49,               /* FormalsList  */
  YYSYMBOL_FormalDecl = 50,                /* FormalDecl  */
  YYSYMBOL_Statements = 51,                /* Statements  */
  YYSYMBOL_Statement = 52,                 /* Statement  */
  YYSYMBOL_53_5 = 53,                      /* @5  */
  YYSYMBOL_54_6 = 54,                      /* @6  */
  YYSYMBOL_55_7 = 55,                      /* $@7  */
  YYSYMBOL_56_8 = 56,                      /* $@8  */
  YYSYMBOL_57_9 = 57,                      /* @9  */
  YYSYMBOL_58_10 = 58,                     /* $@10  */
  YYSYMBOL_Call = 59,                      /* Call  */
  YYSYMBOL_ExpList = 60,                   /* ExpList  */
  YYSYMBOL_Type = 61,                      /* Type  */
  YYSYMBOL_Exp = 62,                       /* Exp  */
  YYSYMBOL_CaseList = 63,                  /* CaseList  */
  YYSYMBOL_CaseDecl = 64,                  /* CaseDecl  */
  YYSYMBOL_OpenScope = 65,                 /* OpenScope  */
  YYSYMBOL_CloseScope = 66                 /* CloseScope  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
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
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
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
#define YYLAST   220

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  65
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  130

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   293


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    52,    52,    52,    59,    64,    70,    73,    76,    69,
      80,    85,    90,    94,    99,   104,   109,   114,   119,   124,
     129,   134,   139,   144,   149,   154,   159,   166,   165,   172,
     171,   180,   185,   191,   194,   197,   200,   190,   204,   209,
     214,   219,   224,   229,   234,   239,   244,   249,   254,   259,
     264,   269,   274,   279,   284,   289,   294,   299,   304,   309,
     314,   319,   324,   329,   334,   338
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "VOID", "INT", "BYTE",
  "B", "BOOL", "TRUE", "FALSE", "RETURN", "IF", "ELSE", "WHILE", "BREAK",
  "CONTINUE", "SC", "COMMA", "ID", "NUM", "STRING", "ASSIGN", "OR", "AND",
  "EQ", "REL", "PLUS_MINUS", "MUL_DIV", "NOT",
  "SWITCH", "CASE", "DEFAULT", "COLON", "RBRACE", "LBRACE", "RPAREN",
  "LPAREN", "SECOND_PRIOR", "FIRST_PRIOR", "$accept", "Program", "@1",
  "Funcs", "FuncDecl", "@2", "$@3", "$@4", "RetType", "Formals",
  "FormalsList", "FormalDecl", "Statements", "Statement", "@5", "@6",
  "$@7", "$@8", "@9", "$@10", "Call", "ExpList", "Type", "Exp", "CaseList",
  "CaseDecl", "OpenScope", "CloseScope", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293
};
#endif

#define YYPACT_NINF (-78)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -78,     4,    73,   -78,   -78,   -78,   -78,   -78,   -78,    73,
     -13,   -78,   -78,   -28,    84,   -19,   -78,    11,    15,   -78,
      84,   -78,   -23,   -78,   -78,   -78,     8,    88,    -2,     9,
      38,    52,    -7,   -78,   -78,     8,   -78,    53,    35,   -78,
     -78,   -78,    54,    86,   -78,   117,   117,   -78,   152,   117,
     117,   -78,   -78,   117,    94,    64,     8,   -78,   -78,   -78,
     -14,   -78,   -78,   116,   -78,   117,   117,   117,   117,   117,
     117,   132,   138,   164,   -78,    70,   187,   117,     8,    76,
     -78,   117,   -78,    24,    37,    57,    92,    83,   -78,   -78,
     -78,   -78,   -78,   117,   193,    78,   -78,   176,     8,   -78,
     -78,    80,   -78,   -78,   108,     8,    87,   -78,   -78,   -78,
     -78,   -78,    -6,     8,   104,    95,   -78,    -6,   -78,    96,
       8,   -78,   -78,   -78,     8,     8,   -78,     8,    98,   -78
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     4,     1,    11,    42,    43,    44,     3,     4,
       0,    10,     5,     0,    12,     0,    13,    14,     0,     6,
       0,    16,     0,    15,    64,     7,     0,     0,     0,     0,
       0,     0,     0,    33,    64,    65,    17,     0,     0,    53,
      54,    24,    48,    50,    52,     0,     0,    49,     0,     0,
       0,    31,    32,     0,     0,     0,     0,    18,     8,    23,
       0,    51,    55,     0,    25,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    39,     0,    40,     0,    65,     0,
      20,     0,    45,    57,    56,    58,    59,    46,    47,    64,
      29,    22,    38,     0,    34,     0,     9,     0,     0,    64,
      41,     0,    19,    21,    26,     0,     0,    27,    65,    64,
      64,    30,     0,     0,     0,     0,    35,    61,    65,     0,
       0,    65,    60,    28,     0,    62,    36,    63,     0,    37
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -78,   -78,   -78,   123,   -78,   -78,   -78,   -78,   -78,   -78,
     113,   -78,   -50,   -32,   -78,   -78,   -78,   -78,   -78,   -78,
     -26,    41,    18,   -10,    27,   -78,   -24,   -77
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,     8,     9,    22,    26,    79,    10,    15,
      16,    17,    35,    36,   110,    99,    55,   101,   121,   128,
      47,    75,    38,    76,   116,   117,    25,    58
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      37,    95,    80,    57,     3,    13,    78,    81,    14,    37,
      56,    24,     5,     6,    53,     7,    19,    48,    27,    28,
      11,    29,    30,    31,   114,   115,    32,    11,    20,    54,
      37,   111,    18,    21,    49,    62,    63,    33,    18,    71,
      72,   123,    34,    73,   126,    50,    57,    66,    67,    68,
      69,    70,    37,    60,    51,    83,    84,    85,    86,    87,
      88,    67,    68,    69,    70,    98,   104,    94,    52,    59,
     125,    97,    37,   108,   127,   105,     4,     5,     6,    37,
       7,   118,    68,    69,    70,   112,   113,    37,     5,     6,
      54,     7,    61,    57,    37,    57,    39,    40,    37,    37,
      77,    37,    39,    40,    41,    92,    42,    43,    44,    96,
      70,   102,    42,    43,    44,   106,    45,    -1,    69,    70,
     107,   109,    45,   119,    46,    39,    40,   120,   124,    74,
      46,   129,    12,    23,   100,    42,    43,    44,    65,    66,
      67,    68,    69,    70,   122,    45,     0,     0,     0,     0,
       0,    82,     0,    46,    65,    66,    67,    68,    69,    70,
      65,    66,    67,    68,    69,    70,     0,    89,    64,     0,
       0,     0,     0,    90,    65,    66,    67,    68,    69,    70,
      91,     0,     0,     0,     0,     0,    65,    66,    67,    68,
      69,    70,   103,     0,     0,     0,     0,     0,    65,    66,
      67,    68,    69,    70,    93,     0,     0,     0,     0,    65,
      66,    67,    68,    69,    70,    65,    66,    67,    68,    69,
      70
};

static const yytype_int8 yycheck[] =
{
      26,    78,    16,    35,     0,    18,    56,    21,    36,    35,
      34,    34,     4,     5,    21,     7,    35,    27,    10,    11,
       2,    13,    14,    15,    30,    31,    18,     9,    17,    36,
      56,   108,    14,    18,    36,    45,    46,    29,    20,    49,
      50,   118,    34,    53,   121,    36,    78,    23,    24,    25,
      26,    27,    78,    18,    16,    65,    66,    67,    68,    69,
      70,    24,    25,    26,    27,    89,    98,    77,    16,    16,
     120,    81,    98,   105,   124,    99,     3,     4,     5,   105,
       7,   113,    25,    26,    27,   109,   110,   113,     4,     5,
      36,     7,     6,   125,   120,   127,     8,     9,   124,   125,
      36,   127,     8,     9,    16,    35,    18,    19,    20,    33,
      27,    33,    18,    19,    20,    35,    28,    25,    26,    27,
      12,    34,    28,    19,    36,     8,     9,    32,    32,    35,
      36,    33,     9,    20,    93,    18,    19,    20,    22,    23,
      24,    25,    26,    27,   117,    28,    -1,    -1,    -1,    -1,
      -1,    35,    -1,    36,    22,    23,    24,    25,    26,    27,
      22,    23,    24,    25,    26,    27,    -1,    35,    16,    -1,
      -1,    -1,    -1,    35,    22,    23,    24,    25,    26,    27,
      16,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,    25,
      26,    27,    16,    -1,    -1,    -1,    -1,    -1,    22,    23,
      24,    25,    26,    27,    17,    -1,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    27,    22,    23,    24,    25,    26,
      27
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    40,    41,     0,     3,     4,     5,     7,    42,    43,
      47,    61,    42,    18,    36,    48,    49,    50,    61,    35,
      17,    18,    44,    49,    34,    65,    45,    10,    11,    13,
      14,    15,    18,    29,    34,    51,    52,    59,    61,     8,
       9,    16,    18,    19,    20,    28,    36,    59,    62,    36,
      36,    16,    16,    21,    36,    55,    65,    52,    66,    16,
      18,     6,    62,    62,    16,    22,    23,    24,    25,    26,
      27,    62,    62,    62,    35,    60,    62,    36,    51,    46,
      16,    21,    35,    62,    62,    62,    62,    62,    62,    35,
      35,    16,    35,    17,    62,    66,    33,    62,    65,    54,
      60,    56,    33,    16,    52,    65,    35,    12,    52,    34,
      53,    66,    65,    65,    30,    31,    63,    64,    52,    19,
      32,    57,    63,    66,    32,    51,    66,    51,    58,    33
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    39,    41,    40,    42,    42,    44,    45,    46,    43,
      47,    47,    48,    48,    49,    49,    50,    51,    51,    52,
      52,    52,    52,    52,    52,    52,    52,    53,    52,    54,
      52,    52,    52,    55,    56,    57,    58,    52,    59,    59,
      60,    60,    61,    61,    61,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    62,
      63,    63,    63,    64,    65,    66
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     0,     2,     0,     0,     0,    13,
       1,     1,     0,     1,     1,     3,     2,     1,     2,     5,
       3,     5,     4,     2,     2,     3,     6,     0,    11,     0,
       8,     2,     2,     0,     0,     0,     0,    13,     4,     3,
       1,     3,     1,     1,     1,     3,     3,     3,     1,     1,
       1,     2,     1,     1,     1,     2,     3,     3,     3,     3,
       2,     1,     3,     4,     0,     0
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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
  case 2: /* @1: %empty  */
#line 52 "parser.ypp"
                {
                    yyval = new Program();
                }
#line 1246 "parser.tab.cpp"
    break;

  case 3: /* Program: @1 Funcs  */
#line 55 "parser.ypp"
                {
                    GlobalSemanticStateHandler::exitProgramRuntimeState();
                }
#line 1254 "parser.tab.cpp"
    break;

  case 4: /* Funcs: %empty  */
#line 60 "parser.ypp"
                {
                    yyval = new Funcs();
                }
#line 1262 "parser.tab.cpp"
    break;

  case 5: /* Funcs: FuncDecl Funcs  */
#line 65 "parser.ypp"
                {
                    yyval = new Funcs();
                }
#line 1270 "parser.tab.cpp"
    break;

  case 6: /* @2: %empty  */
#line 70 "parser.ypp"
                {
                    yyval = new FuncDecl(dynamic_cast<RetType*>(yyvsp[-4]),yyvsp[-3],dynamic_cast<Formals*>(yyvsp[-1]));
                }
#line 1278 "parser.tab.cpp"
    break;

  case 7: /* $@3: %empty  */
#line 73 "parser.ypp"
                {
                    insertFunctionParameters(dynamic_cast<Formals*>(yyvsp[-4]));
                }
#line 1286 "parser.tab.cpp"
    break;

  case 8: /* $@4: %empty  */
#line 76 "parser.ypp"
                {
                    GlobalSemanticStateHandler::exitProgramFuncsState();
                }
#line 1294 "parser.tab.cpp"
    break;

  case 10: /* RetType: Type  */
#line 81 "parser.ypp"
                {
                    yyval = new RetType(dynamic_cast<Type*>(yyvsp[0]));
                }
#line 1302 "parser.tab.cpp"
    break;

  case 11: /* RetType: VOID  */
#line 86 "parser.ypp"
                {
                    yyval = new RetType(yyvsp[0]);
                }
#line 1310 "parser.tab.cpp"
    break;

  case 12: /* Formals: %empty  */
#line 90 "parser.ypp"
                {
                    yyval = new Formals();
                }
#line 1318 "parser.tab.cpp"
    break;

  case 13: /* Formals: FormalsList  */
#line 95 "parser.ypp"
                {
                    yyval = new Formals(dynamic_cast<FormalsList*>(yyvsp[0]));
                }
#line 1326 "parser.tab.cpp"
    break;

  case 14: /* FormalsList: FormalDecl  */
#line 100 "parser.ypp"
                {
                    yyval = new FormalsList(dynamic_cast<FormalDecl*>(yyvsp[0]));
                }
#line 1334 "parser.tab.cpp"
    break;

  case 15: /* FormalsList: FormalDecl COMMA FormalsList  */
#line 105 "parser.ypp"
                {
                    yyval = new FormalsList(dynamic_cast<FormalDecl*>(yyvsp[-2]), dynamic_cast<FormalsList*>(yyvsp[0]));
                }
#line 1342 "parser.tab.cpp"
    break;

  case 16: /* FormalDecl: Type ID  */
#line 110 "parser.ypp"
                {
                    yyval = new FormalDecl(dynamic_cast<Type*>(yyvsp[-1]), yyvsp[0]);
                }
#line 1350 "parser.tab.cpp"
    break;

  case 17: /* Statements: Statement  */
#line 115 "parser.ypp"
                {
                    yyval = new Statements(dynamic_cast<Statement*>(yyvsp[0]));
                }
#line 1358 "parser.tab.cpp"
    break;

  case 18: /* Statements: Statements Statement  */
#line 120 "parser.ypp"
                {
                    yyval = new Statements(dynamic_cast<Statements*>(yyvsp[-1]), dynamic_cast<Statement*>(yyvsp[0]));
                }
#line 1366 "parser.tab.cpp"
    break;

  case 19: /* Statement: LBRACE OpenScope Statements CloseScope RBRACE  */
#line 125 "parser.ypp"
                {
                    yyval = new Statement(dynamic_cast<Statements*>(yyvsp[-2]));
                }
#line 1374 "parser.tab.cpp"
    break;

  case 20: /* Statement: Type ID SC  */
#line 130 "parser.ypp"
                {
                    yyval = new Statement(dynamic_cast<Type*>(yyvsp[-2]),yyvsp[-1]);
                }
#line 1382 "parser.tab.cpp"
    break;

  case 21: /* Statement: Type ID ASSIGN Exp SC  */
#line 135 "parser.ypp"
                {
                    yyval = new Statement(dynamic_cast<Type*>(yyvsp[-4]),yyvsp[-3], dynamic_cast<Exp*>(yyvsp[-1]));
                }
#line 1390 "parser.tab.cpp"
    break;

  case 22: /* Statement: ID ASSIGN Exp SC  */
#line 140 "parser.ypp"
                {
                    yyval = new Statement(yyvsp[-3], dynamic_cast<Exp*>(yyvsp[-1]));
                }
#line 1398 "parser.tab.cpp"
    break;

  case 23: /* Statement: Call SC  */
#line 145 "parser.ypp"
                {
                    yyval = new Statement(dynamic_cast<Call*>(yyvsp[-1]));
                }
#line 1406 "parser.tab.cpp"
    break;

  case 24: /* Statement: RETURN SC  */
#line 150 "parser.ypp"
                {
                    yyval = new Statement("VOID");
                }
#line 1414 "parser.tab.cpp"
    break;

  case 25: /* Statement: RETURN Exp SC  */
#line 155 "parser.ypp"
                {
                    yyval = new Statement(dynamic_cast<Exp*>(yyvsp[-1]));
                }
#line 1422 "parser.tab.cpp"
    break;

  case 26: /* Statement: IF LPAREN Exp RPAREN OpenScope Statement  */
#line 160 "parser.ypp"
                {
                    yyval = new Statement("if", dynamic_cast<Exp*>(yyvsp[-3]));
                    GlobalSemanticStateHandler::closeScope();
                }
#line 1431 "parser.tab.cpp"
    break;

  case 27: /* @5: %empty  */
#line 166 "parser.ypp"
                {
                    yyval = new Statement("if else", dynamic_cast<Exp*>(yyvsp[-4]));
                    GlobalSemanticStateHandler::closeScope();
                }
#line 1440 "parser.tab.cpp"
    break;

  case 29: /* @6: %empty  */
#line 172 "parser.ypp"
                {
                    yyval = new Statement("while", dynamic_cast<Exp*>(yyvsp[-1]));
                    GlobalSemanticStateHandler::enterLoopState();
                }
#line 1449 "parser.tab.cpp"
    break;

  case 30: /* Statement: WHILE LPAREN Exp RPAREN @6 OpenScope Statement CloseScope  */
#line 176 "parser.ypp"
                {
                    GlobalSemanticStateHandler::exitLoopState();
                }
#line 1457 "parser.tab.cpp"
    break;

  case 31: /* Statement: BREAK SC  */
#line 181 "parser.ypp"
                {
                    yyval = new Statement(yyvsp[-1]);
                }
#line 1465 "parser.tab.cpp"
    break;

  case 32: /* Statement: CONTINUE SC  */
#line 186 "parser.ypp"
                {
                    yyval = new Statement(yyvsp[-1]);
                }
#line 1473 "parser.tab.cpp"
    break;

  case 33: /* $@7: %empty  */
#line 191 "parser.ypp"
                {
                    GlobalSemanticStateHandler::enterSwitchState();
                }
#line 1481 "parser.tab.cpp"
    break;

  case 34: /* $@8: %empty  */
#line 194 "parser.ypp"
                {
                    new Exp(dynamic_cast<Exp*>(yyvsp[0]), "switch");
                }
#line 1489 "parser.tab.cpp"
    break;

  case 35: /* @9: %empty  */
#line 197 "parser.ypp"
                {
                    yyval = new Statement(dynamic_cast<Exp*>(yyvsp[-5]), dynamic_cast<CaseList*>(yyvsp[0]));
                }
#line 1497 "parser.tab.cpp"
    break;

  case 36: /* $@10: %empty  */
#line 200 "parser.ypp"
                {
                    GlobalSemanticStateHandler::exitSwitchState();
                }
#line 1505 "parser.tab.cpp"
    break;

  case 38: /* Call: ID LPAREN ExpList RPAREN  */
#line 205 "parser.ypp"
                {
                    yyval = new Call(yyvsp[-3], dynamic_cast<ExpList*>(yyvsp[-1]));
                }
#line 1513 "parser.tab.cpp"
    break;

  case 39: /* Call: ID LPAREN RPAREN  */
#line 210 "parser.ypp"
                {
                    yyval = new Call(yyvsp[-2]);
                }
#line 1521 "parser.tab.cpp"
    break;

  case 40: /* ExpList: Exp  */
#line 215 "parser.ypp"
                {
                    yyval = new ExpList(dynamic_cast<Exp*>(yyvsp[0]));
                }
#line 1529 "parser.tab.cpp"
    break;

  case 41: /* ExpList: Exp COMMA ExpList  */
#line 220 "parser.ypp"
                {
                    yyval = new ExpList(dynamic_cast<Exp*>(yyvsp[-2]), dynamic_cast<ExpList*>(yyvsp[0]));
                }
#line 1537 "parser.tab.cpp"
    break;

  case 42: /* Type: INT  */
#line 225 "parser.ypp"
                {
                    yyval = new Type(yyvsp[0]);
                }
#line 1545 "parser.tab.cpp"
    break;

  case 43: /* Type: BYTE  */
#line 230 "parser.ypp"
                {
                    yyval = new Type(yyvsp[0]);
                }
#line 1553 "parser.tab.cpp"
    break;

  case 44: /* Type: BOOL  */
#line 235 "parser.ypp"
                {
                    yyval = new Type(yyvsp[0]);
                }
#line 1561 "parser.tab.cpp"
    break;

  case 45: /* Exp: LPAREN Exp RPAREN  */
#line 240 "parser.ypp"
                {
                    yyval = new Exp(dynamic_cast<Exp*>(yyvsp[-1]));
                }
#line 1569 "parser.tab.cpp"
    break;

  case 46: /* Exp: Exp PLUS_MINUS Exp  */
#line 245 "parser.ypp"
                {
                    yyval = new Exp(dynamic_cast<Exp*>(yyvsp[-2]),yyvsp[-1],dynamic_cast<Exp*>(yyvsp[0]), "PLUS_MINUS");
                }
#line 1577 "parser.tab.cpp"
    break;

  case 47: /* Exp: Exp MUL_DIV Exp  */
#line 250 "parser.ypp"
                {
                    yyval = new Exp(dynamic_cast<Exp*>(yyvsp[-2]), yyvsp[-1], dynamic_cast<Exp*>(yyvsp[0]), "MUL_DIV");
                }
#line 1585 "parser.tab.cpp"
    break;

  case 48: /* Exp: ID  */
#line 255 "parser.ypp"
                {
                    yyval = new Exp(yyvsp[0]);
                }
#line 1593 "parser.tab.cpp"
    break;

  case 49: /* Exp: Call  */
#line 260 "parser.ypp"
                {
                    yyval = new Exp(dynamic_cast<Call*>(yyvsp[0]));
                }
#line 1601 "parser.tab.cpp"
    break;

  case 50: /* Exp: NUM  */
#line 265 "parser.ypp"
                {
                    yyval = new Exp(yyvsp[0], "NUM");
                }
#line 1609 "parser.tab.cpp"
    break;

  case 51: /* Exp: NUM B  */
#line 270 "parser.ypp"
                {
                    yyval = new Exp(yyvsp[-1], "BYTE");
                }
#line 1617 "parser.tab.cpp"
    break;

  case 52: /* Exp: STRING  */
#line 275 "parser.ypp"
                {
                    yyval = new Exp(yyvsp[0], "STRING");
                }
#line 1625 "parser.tab.cpp"
    break;

  case 53: /* Exp: TRUE  */
#line 280 "parser.ypp"
                {
                    yyval = new Exp(yyvsp[0], "BOOL");
                }
#line 1633 "parser.tab.cpp"
    break;

  case 54: /* Exp: FALSE  */
#line 285 "parser.ypp"
                {
                    yyval = new Exp(yyvsp[0], "BOOL");
                }
#line 1641 "parser.tab.cpp"
    break;

  case 55: /* Exp: NOT Exp  */
#line 290 "parser.ypp"
                {
                    yyval = new Exp(yyvsp[-1], dynamic_cast<Exp*>(yyvsp[0]));
                }
#line 1649 "parser.tab.cpp"
    break;

  case 56: /* Exp: Exp AND Exp  */
#line 295 "parser.ypp"
                {
                    yyval = new Exp(dynamic_cast<Exp*>(yyvsp[-2]), yyvsp[-1], dynamic_cast<Exp*>(yyvsp[0]), "AND");
                }
#line 1657 "parser.tab.cpp"
    break;

  case 57: /* Exp: Exp OR Exp  */
#line 300 "parser.ypp"
                {
                    yyval = new Exp(dynamic_cast<Exp*>(yyvsp[-2]), yyvsp[-1], dynamic_cast<Exp*>(yyvsp[0]), "OR");
                }
#line 1665 "parser.tab.cpp"
    break;

  case 58: /* Exp: Exp EQ Exp  */
#line 305 "parser.ypp"
                {
                    yyval = new Exp(dynamic_cast<Exp*>(yyvsp[-2]), yyvsp[-1], dynamic_cast<Exp*>(yyvsp[0]), "EQ");
                }
#line 1673 "parser.tab.cpp"
    break;

  case 59: /* Exp: Exp REL Exp  */
#line 310 "parser.ypp"
                {
                    yyval = new Exp(dynamic_cast<Exp*>(yyvsp[-2]), yyvsp[-1], dynamic_cast<Exp*>(yyvsp[0]), "REL");
                }
#line 1681 "parser.tab.cpp"
    break;

  case 60: /* CaseList: CaseDecl CaseList  */
#line 315 "parser.ypp"
                {
                    yyval = new CaseList(dynamic_cast<CaseDecl*>(yyvsp[-1]), dynamic_cast<CaseList*>(yyvsp[0]));
                }
#line 1689 "parser.tab.cpp"
    break;

  case 61: /* CaseList: CaseDecl  */
#line 320 "parser.ypp"
                {
                    yyval = new CaseList(dynamic_cast<CaseDecl*>(yyvsp[0]));
                }
#line 1697 "parser.tab.cpp"
    break;

  case 62: /* CaseList: DEFAULT COLON Statements  */
#line 325 "parser.ypp"
                {
                    yyval = new CaseList(dynamic_cast<Statements*>(yyvsp[0]));
                }
#line 1705 "parser.tab.cpp"
    break;

  case 63: /* CaseDecl: CASE NUM COLON Statements  */
#line 330 "parser.ypp"
                {
                    yyval = new CaseDecl(new Exp(yyvsp[-2], "NUM"), dynamic_cast<Statements*>(yyvsp[0]));
                }
#line 1713 "parser.tab.cpp"
    break;

  case 64: /* OpenScope: %empty  */
#line 334 "parser.ypp"
                {
                    GlobalSemanticStateHandler::openScope();
                }
#line 1721 "parser.tab.cpp"
    break;

  case 65: /* CloseScope: %empty  */
#line 338 "parser.ypp"
                {
                    GlobalSemanticStateHandler::closeScope();
                }
#line 1729 "parser.tab.cpp"
    break;


#line 1733 "parser.tab.cpp"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

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


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 342 "parser.ypp"


/* Code section */

int main() {
    return yyparse();
}

int yyerror(const char * message) {
    output::errorSyn(yylineno);
    exit(0);
}
