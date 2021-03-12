############################################################################
#===========================================================================


## Project setup.
##==========================================================================

# The directories in which source files reside.
SRCDIR    = src
SRCDIRS   = $(SRCDIR)/ $(SRCDIR)/*/

BUILDDIR  = build

# The executable file name.
PROJECT   = crypto

## The linker options.
##==========================================================================
LIBS      =

# The options used in linking as well as in any direct use of ld.
LDFLAGS   =

## Flags and compiler options.
##==========================================================================

# The pre-processor and compiler options.
CFLAGS  = -g -std=c99

# The compiler.
CC     = gcc

# The command used to delete file.
RM     = rm -f

ETAGS = etags
ETAGSFLAGS =

CTAGS = ctags
CTAGSFLAGS =

## Stable Section: usually no need to be changed. But you can add more.
##==========================================================================
SOURCES = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)*,.c)))
HEADERS = $(foreach d,$(SRCDIRS),$(wildcard $(addprefix $(d)*,.h)))
OBJS    = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))
DEPS    = $(OBJS:.o=.d)

## Define some useful variables.
DEP_OPT = $(shell if `$(CC) --version | grep "GCC" >/dev/null`; then echo "-MM -MP"; else echo "-M"; fi)
DEPEND  = $(CC)  $(DEP_OPT) $(CFLAGS)
COMPILE = $(CC)  $(CFLAGS)  -c
LINK    = $(CC)  $(CFLAGS)  $(LDFLAGS)

.PHONY: all objs tags ctags clean distclean help show

# Delete the default suffixes
.SUFFIXES:

all: $(PROJECT)

# Rules for creating dependency files (.d).
#------------------------------------------

$(BUILDDIR)/%.d:$(SRCDIR)/%.c
	@echo -n $(dir $<) > $@
	@$(DEPEND) $< >> $@

# Rules for generating object files (.o).
#----------------------------------------
objs:$(OBJS)

$(BUILDDIR)/%.o:$(SRCDIR)/%.c
	$(COMPILE) $< -o $@

# Rules for generating the tags.
#-------------------------------------
tags: $(HEADERS) $(SOURCES)
	$(ETAGS) $(ETAGSFLAGS) $(HEADERS) $(SOURCES)

ctags: $(HEADERS) $(SOURCES)
	$(CTAGS) $(CTAGSFLAGS) $(HEADERS) $(SOURCES)

# Rules for generating the executable.
#-------------------------------------
$(PROJECT):$(OBJS)
	$(LINK)   $(OBJS) $(LIBS) -o $@
	@echo Type ./$@ to execute the program.

ifndef NODEP
  sinclude $(DEPS)
endif

clean:
	$(RM) $(OBJS) $(PROJECT) $(PROJECT).exe

distclean: clean
	$(RM) $(DEPS) TAGS

# Show help.
help:
	@echo 'Usage: make [TARGET]'
	@echo 'TARGETS:'
	@echo '  all       (=make) compile and link.'
	@echo '  NODEP=yes make without generating dependencies.'
	@echo '  objs      compile only (no linking).'
	@echo '  tags      create tags for Emacs editor.'
	@echo '  ctags     create ctags for VI editor.'
	@echo '  clean     clean objects and the executable file.'
	@echo '  distclean clean objects, the executable and dependencies.'
	@echo '  show      show variables (for debug use only).'
	@echo '  help      print this message.'

# Show variables (for debug use only.)
show:
	@echo 'PROJECT  :' $(PROJECT)
	@echo 'SRCDIRS  :' $(SRCDIRS)
	@echo 'HEADERS  :' $(HEADERS)
	@echo 'SOURCES  :' $(SOURCES)
	@echo 'OBJS     :' $(OBJS)
	@echo 'DEPS     :' $(DEPS)
	@echo 'DEPEND   :' $(DEPEND)
	@echo 'COMPILE  :' $(COMPILE)
	@echo 'LINK     :' $(LINK)

#===========================================================================
############################################################################