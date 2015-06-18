SRCDIR := $(shell pwd)
BUILDDIR := $(SRCDIR)/build
VERSION := $(shell cat VERSION)
DEVICE := "12f675"
BUILDIMAGE := "ewoutp/sdcc-docker"

.PHONY: clean prepare compile

all: prepare build_all

clean:
	rm -Rf $(BUILDDIR)

prepare:
	mkdir -p $(BUILDDIR)/obj
	mkdir -p $(BUILDDIR)/bin

compile:
	docker run -t \
		-v $(SRCDIR):$(SRCDIR) \
		-w $(SRCDIR) \
		$(BUILDIMAGE) \
		sdcc -D$(COMMON) -D$(COLORS) -mpic14 -p$(DEVICE) -o $(BUILDDIR)/obj/main-$(COMMON)-$(COLORS).o -c $(SRCDIR)/src/main.c
	docker run -t \
		-v $(SRCDIR):$(SRCDIR) \
		-w $(SRCDIR) \
		$(BUILDIMAGE) \
		sdcc -mpic14 -p$(DEVICE) -Wl-c -Wl-m -o $(BUILDDIR)/bin/Bs1ForSignals-$(COMMON)-$(COLORS)-$(VERSION).hex $(BUILDDIR)/obj/main-$(COMMON)-$(COLORS).o
	cp -f $(BUILDDIR)/bin/*.hex $(BUILDDIR)

build_all:
	COMMON="CMNGND" COLORS="COLORS2" ${MAKE} -B -s compile
	COMMON="CMNGND" COLORS="COLORS4" ${MAKE} -B -s compile
	COMMON="CMNPLUS" COLORS="COLORS2" ${MAKE} -B -s compile
	COMMON="CMNPLUS" COLORS="COLORS4" ${MAKE} -B -s compile
