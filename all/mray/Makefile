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

xray:
	make -C ../../cc/keyboard $(opfad)vkbd.o
	make -C ../../xwindow/keyboard $(opfad)xkbd.o
	make -C ../../cc/screen $(opfad)libscr.a
	make -C ../../xwindow/screen $(opfad)xscr.o
	make -C ../../cc/class $(opfad)vektor.o
	make -C ../../cc/class $(opfad)ray.o
	make $(ppfad)xray

$(ppfad)xray: $(opfad)xray.o $(opfad)vkbd.o $(opfad)xkbd.o $(opfad)libscr.a $(opfad)xscr.o $(opfad)vektor.o $(opfad)ray.o
	$(comp) $+ $(lopts) -o $@

$(opfad)xray.o: xray.cc
	$(comp) $< -c $(copts) -o $@

xraythread:
	make -C ../../cc/keyboard $(opfad)vkbd.o
	make -C ../../xwindow/keyboard $(opfad)xkbd.o
	make -C ../../cc/screen $(opfad)libscr.a
	make -C ../../xwindow/screen $(opfad)xscr.o
	make -C ../../cc/class $(opfad)vektor.o
	make -C ../../cc/class $(opfad)ray.o
	make $(ppfad)xraythread

$(ppfad)xraythread: $(opfad)xraythread.o $(opfad)vkbd.o $(opfad)xkbd.o $(opfad)libscr.a $(opfad)xscr.o $(opfad)vektor.o $(opfad)ray.o
	$(comp) $+ $(lopts) -o $@

$(opfad)xraythread.o: xraythread.cc
	$(comp) $< -c $(copts) -o $@

fbray:
	make -C ../../cc/keyboard $(opfad)vkbd.o
	make -C ../../xwindow/keyboard $(opfad)xkbd.o
	make -C ../../cc/screen $(opfad)libscr.a
	make -C ../../framebuffer/screen $(opfad)fbscr.o
	make -C ../../cc/class $(opfad)vektor.o
	make -C ../../cc/class $(opfad)ray.o
	make $(ppfad)fbray

$(ppfad)fbray: $(opfad)fbray.o $(opfad)vkbd.o $(opfad)xkbd.o $(opfad)libscr.a $(opfad)fbscr.o $(opfad)vektor.o $(opfad)ray.o
	$(comp) $+ $(lopts) -o $@

$(opfad)fbray.o: fbray.cc
	$(comp) $< -c $(copts) -o $@

symmetriegruppen:
	make -C ../../cc/class $(opfad)vektor.o
	make -C ../../cc/class $(opfad)gruppe.o
	make -C ../../cc/class $(opfad)ray.o
	make -C ../../cc/keyboard $(opfad)vkbd.o
	make -C ../../cc/screen $(opfad)libscr.a
	make -C ../../conio $(opfad)vektorcon.o
	make -C ../../conio $(opfad)gruppecon.o
	make -C ../../xwindow/keyboard $(opfad)xkbd.o
	make -C ../../xwindow/screen $(opfad)xscr.o
	make $(ppfad)symmetriegruppen

$(ppfad)symmetriegruppen: $(opfad)symmetriegruppen.o $(opfad)vkbd.o $(opfad)xkbd.o $(opfad)libscr.a $(opfad)xscr.o $(opfad)vektor.o $(opfad)gruppe.o $(opfad)vektorcon.o $(opfad)gruppecon.o $(opfad)ray.o
	$(comp) $+ $(lopts) -o $@

$(opfad)symmetriegruppen.o: symmetriegruppen.cc
	$(comp) $< -c $(copts) -o $@

allclean:
	rm -f $(opfad)*
	rm -f $(ppfad)*

