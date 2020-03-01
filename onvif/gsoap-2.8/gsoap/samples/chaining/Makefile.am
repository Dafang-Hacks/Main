####### This is the input file for automake, which will generate Makefile.in ##########
## not a GNU package. You can remove this line, if
## you have all needed files, that a GNU package needs
AUTOMAKE_OPTIONS = foreign 1.4

noinst_PROGRAMS = chaining

include ../Makefile.defines
# include ../Makefile.c_rules

envC.c:		env.h
	$(SOAP) -c -CS -penv -I$(top_srcdir)/gsoap/import:$(top_srcdir)/gsoap $(SOAP_FLAGS) $<
qServerLib.c:	quote.h
	$(SOAP) -c -S -np q -I$(top_srcdir)/gsoap/import:$(top_srcdir)/gsoap $(SOAP_FLAGS) $<
cServerLib.c:	calc.h
	$(SOAP) -c -S -np c -I$(top_srcdir)/gsoap/import:$(top_srcdir)/gsoap $(SOAP_FLAGS) $<

BUILT_SOURCES = qServerLib.c cServerLib.c envC.c

CLEANFILES = *.req.xml *.res.xml *.nsmap *.xsd *.wsdl envStub.h envH.h envC.c cStub.h cH.h cC.c Calc*Proxy.h Calc*Proxy.c Calc*Object.h Calc*Service.h Calc*Service.c cClient.c cServer.c cClientLib.c cServerLib.c qStub.h qH.h qC.c Quote*Proxy.h Quote*Proxy.c Quote*Object.h Quote*Service.h Quote*Service.c QuoteClient.c QuoteServer.c qClientLib.c qServerLib.c *~

SOAP_NS = c.nsmap

chaining_SOURCES = chaining.c quote.h calc.h env.h qServerLib.c cServerLib.c envC.c
chaining_LDADD = $(SOAP_C_LIB) $(SAMPLE_EXTRA_LIBS)
