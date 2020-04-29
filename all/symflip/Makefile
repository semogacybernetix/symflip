# ------------ Makefile mray -------------------------------------------

# --------------------------- Umgebungsvariablen ------------------------------

ifndef PLATFORM
$(error you must set PLATFORM variable (linux))
endif

ifndef CCSTANDARD
$(error you must set CCSTANDARD variable (c++11))
endif

opfad=$(HOME)/bin/$(PLATFORM)/o/
ppfad=$(HOME)/bin/$(PLATFORM)/sbin/

comp=g++

# -------------------------- Makeoptionen ------------------------------------

copts=-std=$(CCSTANDARD) -Wall -Wextra -Wconversion -pedantic-errors -O2
lopts=-L$(opfad) -lscr -lpthread -ljpeg -lX11

# -------------------------- Ziele -------------------------------------------

symflip:
	make -C ../../cc/class $(opfad)vektor.o
	make -C ../../cc/class $(opfad)gruppe.o
	make -C ../../cc/class $(opfad)ray.o
	make -C ../../cc/class $(opfad)game.o
	make -C ../../cc/keyboard $(opfad)vkbd.o
	make -C ../../cc/screen $(opfad)libscr.a
	make -C ../../conio $(opfad)vektorcon.o
	make -C ../../conio $(opfad)gruppecon.o
	make -C ../../xwindow/keyboard $(opfad)xkbd.o
	make -C ../../xwindow/screen $(opfad)xscr.o
	make $(ppfad)symflip

$(ppfad)symflip: $(opfad)symflip.o $(opfad)vektor.o $(opfad)gruppe.o $(opfad)ray.o $(opfad)game.o $(opfad)vkbd.o $(opfad)libscr.a $(opfad)vektorcon.o $(opfad)gruppecon.o $(opfad)xkbd.o $(opfad)xscr.o
	$(comp) $+ $(lopts) -o $@

$(opfad)symflip.o: symflip.cc
	$(comp) $< -c $(copts) -o $@

allclean:
	rm -f $(opfad)*
	rm -f $(ppfad)*
