####################################################################################################
#                                     Make file LargeInt 	                                   #
####################################################################################################

SUBDIRS:= src

all: 
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir all; \
	done
	
clean:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done