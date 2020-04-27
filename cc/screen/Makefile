# ------------ Makefile screen -------------------------------------------

# --------------------------- Umgebung testen  ------------------------------

ifndef PLATFORM
$(error you must set PLATFORM variable (linux))
endif

ifndef CCSTANDARD
$(error you must set CCSTANDARD variable (c++14))
endif

opfad=$(HOME)/bin/$(PLATFORM)/o/
ppfad=$(HOME)/bin/$(PLATFORM)/sbin/

comp=g++

# -------------------------- Make Optionen  ------------------------------------

copts=-std=$(CCSTANDARD) -Wall -Wextra -Wconversion -pedantic-errors -O2

mods=\
    vscr\
    bmpscr\
    jpegscr\
    psscr\

pmods= $(addprefix $(opfad), $(mods))
pobjs= $(addsuffix .o, $(pmods))

# -------------------------- Ziele ------------------------------------------

$(opfad)%.o: %.cc %.h
	$(comp) $< -c $(copts) -o $@

all: $(pobjs) $(opfad)libscr.a

allclean:
	rm -f $(opfad)*
	rm -f $(ppfad)*

clean:
	rm -f $(opfad)libscr.a
	rm -f $(pobjs)
	rm -f $(opfad)jpeg.o
	rm -f $(opfad)leedct.o

rebuild: clean all

$(opfad)libscr.a: $(pobjs)
	ar rs $@ $(pobjs)

$(opfad)jpegscr.o: $(opfad)jpeg.o $(opfad)leedct.o

$(opfad)jpeg.o:
	make -C ../jpeg $@

$(opfad)leedct.o:
	make -C ../jpeg $@
