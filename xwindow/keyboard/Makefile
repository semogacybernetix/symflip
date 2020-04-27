# ------------ Makefile xwindow/keyboard -------------------------------------------

# --------------------------- Umgebung testen  ------------------------------

ifndef PLATFORM
$(error you must set PLATFORM variable (linux))
endif

ifndef CCSTANDARD
$(error you must set CCSTANDARD variable (c++11))
endif

opfad=$(HOME)/bin/$(PLATFORM)/o/
ppfad=$(HOME)/bin/$(PLATFORM)/sbin/

comp=g++

# -------------------------- Make Optionen  ------------------------------------

copts=-std=$(CCSTANDARD) -Wall -Wextra -Wconversion -pedantic-errors -O2

#---------------------------- Ziele --------------------------------------------

mods=\
  xkbd\

pmods= $(addprefix $(opfad), $(mods))
pobjs= $(addsuffix .o, $(pmods))

$(opfad)%.o: %.cc %.h
	$(comp) $< -c $(copts) -o $@

all: $(pobjs)

allclean:
	rm -f $(opfad)*
	rm -f $(ppfad)*

clean:
	rm -f $(pobjs)

rebuild: clean all
