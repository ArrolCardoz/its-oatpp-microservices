

# Top-level Makefile: delegate to subdirectory Makefiles

SUBDIRS := userServer issueServer searchServer commentServer

.PHONY: all $(SUBDIRS) clean test help servers

servers: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ server

clean:
	@for d in $(SUBDIRS); do \
		$(MAKE) -C $$d clean || exit $$?; \
	done

help:
	@echo "Available targets:"; \
	echo "  clean   - run 'make clean' in each subproject"; \
	echo "  servers - alias for all"
